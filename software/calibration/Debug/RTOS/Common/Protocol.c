///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Protocol.c
/// \brief		Système d'échanges de messages entre processeurs
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		30/10/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>
#define DEBUG_NAME "PROTO"
#ifdef RTOS_DEBUG_PROTOCOL
	#define DEBUG_LEVEL RTOS_DEBUG_PROTOCOL
#endif
#include <Common/DebugLevel.h>



typedef struct {

	Fifo		*recvFifo[RTOS_PROTOCOL_RECV_FIFO_MAX];
	struct {
		u16		addr;
		u16		mask;
		Func2	cb;
		void	*cbArg;
	}recvAssoc[RTOS_PROTOCOL_RECV_ENTRY_MAX];

	struct {
		u16		addr;
		u16		mask;
		Fifo	*fifo;
	}sendAssoc[RTOS_PROTOCOL_SEND_ENTRY_MAX];

	Task		task;
}RtosProtocol;

RtosProtocol rtosProtocol;



int protocolTask( void *dummy);



int protocolInit()
{
	memset(&rtosProtocol,0,sizeof(RtosProtocol));

	rtosProtocol.task = taskInit( (Func1)protocolTask, NULL, RTOS_TASK_PERIOD, 0);
	taskStart( rtosProtocol.task);

	return ISUCCESS;
}//protocolInit



u16 protocolCrc( u8 *msg, ProtocolSize size)
{
	u16 crc = 0;
	#ifndef PROTOCOL_SIZE_32BITS
		while( --size>4 ) // Le crc n'est pas encore dans la trame
			crc += msg[size];
	#else
		while( --size>6 ) // Le crc n'est pas encore dans la trame
			crc += msg[size];
	#endif
	return crc;
}//protocolCrc



int sendMsg( u16 addr, void *msg, ProtocolSize size)
{
	RtosProtocol *obj = &rtosProtocol;

	if(size>PROTOCOL_MSG_SIZE_MAX) {
		warning2("Send size:%d", size);
	}

	((ProtocolHeader*)msg)->fanion = PROTOCOL_FANION;
	((ProtocolHeader*)msg)->id = addr;
	((ProtocolHeader*)msg)->size = size;
	((ProtocolHeader*)msg)->crc = protocolCrc(msg,size);

	int ret = IERROR;
	u16 i;
	for( i=0; i<RTOS_PROTOCOL_SEND_ENTRY_MAX; i++)
		if( (obj->sendAssoc[i].fifo!=NULL) && ((addr&obj->sendAssoc[i].mask)==obj->sendAssoc[i].addr) )
			ret = fifoEnqueue(obj->sendAssoc[i].fifo,msg,size);
	if( ret==ISUCCESS ) {
		info4("Send msg");
	}
	else {
		warning2("Send error "STRX16, addr);
	}

	return ret;
}//sendMsg



int protocolAddRecvFifo( Fifo	*fifo		///<
							)
{
	RtosProtocol *obj = &rtosProtocol;

	if(fifo->max+10<PROTOCOL_MSG_SIZE_MAX) {
		warning1("RX: Fifo size smaller than msg max size: %d, %d", fifo->max, PROTOCOL_MSG_SIZE_MAX);
	}


	u16 i;
	for( i=0; i<RTOS_PROTOCOL_RECV_FIFO_MAX; i++)
	{
		if( obj->recvFifo[i]==NULL )
		{
			obj->recvFifo[i] = fifo;
			info4("Fifo added %x", fifo);
			return ISUCCESS;
		}
	}
	error("Not enough RECV_FIFO");

	return IERROR;
}//protocolAddRecvFifo


int protocolAddRecvMsg(	u16		addr,		///<
						u16		mask,		///<
						Func2	cb,			///<
						void	*cbArg		///<
						 )
{
	RtosProtocol *obj = &rtosProtocol;

	u16 i;
	for( i=0; i<RTOS_PROTOCOL_RECV_ENTRY_MAX; i++)
	{
		if( obj->recvAssoc[i].cb==NULL )
		{
			obj->recvAssoc[i].addr = addr;
			obj->recvAssoc[i].mask = mask;
			obj->recvAssoc[i].cb = cb;
			obj->recvAssoc[i].cbArg = cbArg;
			info4("Recv msg added %x", addr);
			return ISUCCESS;
		}
	}
	error("Not enough RECV_ENTRY");

	return IERROR;
}//protocolAddRecvMsg


int protocolAddSendFifo(u16		addr,		///<
						u16		mask,		///<
						Fifo	*fifo		///<
						 )
{
	RtosProtocol *obj = &rtosProtocol;

	if(fifo->max+10<PROTOCOL_MSG_SIZE_MAX) {
		warning1("TX: Fifo size smaller than msg max size: %d, %d", fifo->max, PROTOCOL_MSG_SIZE_MAX);
	}

	u16 i;
	for( i=0; i<RTOS_PROTOCOL_SEND_ENTRY_MAX; i++)
	{
		if( obj->sendAssoc[i].fifo==NULL )
		{
			obj->sendAssoc[i].addr = addr;
			obj->sendAssoc[i].mask = mask;
			obj->sendAssoc[i].fifo = fifo;
			info4("Send msg added %x", addr);
			return ISUCCESS;
		}
	}
	error("Not enough SEND_ENTRY");

	return IERROR;
}//protocolAddSendMsg





int protocolTask( void *dummy)
{
	ignoreArg(dummy);
	RtosProtocol *obj = &rtosProtocol;

	u16 i;
	for( i=0; obj->recvFifo[i]!=NULL && i<RTOS_PROTOCOL_RECV_FIFO_MAX; i++ )
	{
		Fifo *f = obj->recvFifo[i];
		do {
			if( getFifoAvailable(f)<sizeof(ProtocolHeader))
				break;

			if( fifoPeekChar(f)!=PROTOCOL_FANION ) {
				fifoDequeueQuick(f,1);
				info5(".");
				continue;
			}

			ProtocolHeader h;
			if( fifoPeek(f,&h,sizeof(ProtocolHeader)) != ISUCCESS ) {
				error("Dequeue");
				break;
			}

			if( h.size>PROTOCOL_MSG_SIZE_MAX || h.size<sizeof(ProtocolHeader) ) {
				fifoDequeueQuick(f,1);
				warning1("Max size %u", h.size);
				continue;
			}

			u8 msg[PROTOCOL_MSG_SIZE_MAX];
			if( fifoDequeue(f,msg,h.size) != ISUCCESS )
				break;

			if( h.crc!=protocolCrc(msg,h.size) ) {
				fifoDequeueQuick(f,1);
				warning1("CRC %d %d", protocolCrc(msg,h.size), h.crc);
				continue;
			}

			info4("New msg %x", h.id);

			u16 j;
			int ret = -10;
			for( j=0; j<RTOS_PROTOCOL_RECV_ENTRY_MAX; j++)
				if( (obj->recvAssoc[j].cb!=NULL) && ((h.id&obj->recvAssoc[j].mask)==obj->recvAssoc[j].addr) )
					ret = obj->recvAssoc[j].cb( obj->recvAssoc[j].cbArg,  msg);
			if( ret != ISUCCESS ) {
				warning2("return %d; ID:%x", ret, h.id);
			}

			break;

		}while(1);//while fanion
	}//while fifo

	return ISUCCESS;
}//protocolTask



