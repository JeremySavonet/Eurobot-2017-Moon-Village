///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Uart.rpi.cpp
/// \brief		Gestion de l'UART
///
/// \author		Etienne BOILLEY
/// \date		21/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>
#include <QSerialPort>



QSerialPort *rtosUart[RTOS_UART_NB] = {};



int uartTask( Uart *obj);



int uartInit( Uart *obj, UartId id, u32 baudrate, u8 rx[], Size rxSize, u8 tx[], Size txSize)
{
	// on clear
	memset( obj, 0, sizeof(Uart));
	
	// on init les fifos
	fifoInit( &obj->rx, rx, rxSize);
	fifoInit( &obj->tx, tx, txSize);

	u16 i;
	for(i=0;i<RTOS_UART_NB;i++)
		if(rtosUart[i]==NULL)
			break;
	if(i==RTOS_UART_NB)
	{
		rtosError("UART\tNot enough\r\n");
		return IERROR;
	}
	obj->uartId = i;
	obj->uartName = id;

	rtosUart[obj->uartId] = new QSerialPort();
	rtosUart[obj->uartId]->setPortName(obj->uartName);

	if(!rtosUart[obj->uartId]->setBaudRate(baudrate)
			|| !rtosUart[obj->uartId]->setDataBits(QSerialPort::Data8)
			|| !rtosUart[obj->uartId]->setParity(QSerialPort::NoParity)
			|| !rtosUart[obj->uartId]->setStopBits(QSerialPort::OneStop)
			|| !rtosUart[obj->uartId]->setFlowControl(QSerialPort::NoFlowControl) )
	{
		rtosError("UART\tConfig (%s)\r\n",obj->uartName);
		return IERROR;
	}

	if(!rtosUart[obj->uartId]->open(QIODevice::ReadWrite) )
	{
		rtosError("UART\tOpen (%s)\r\n",obj->uartName);
		return IERROR;
	}

	obj->task = taskInit( (Func1)uartTask, obj, RTOS_TASK_PERIOD, 0);
	taskStart( obj->task);
	
	return ISUCCESS;
}//uartInit


int uartSetBrg( Uart *obj, u32 baudrate)
{
	rtosUart[obj->uartId]->setBaudRate(baudrate);
	return ISUCCESS;
}//uartSetBrg


int uartTask( Uart *obj)
{
	uartFlush(obj);
	return ISUCCESS;
}//uartTask


int uartFlush( Uart *obj)
{
	u8 buffer[10000];
	u32 s;

	s = getFifoAvailable(&obj->tx);
	if(s>10000)
		s = 10000;
	fifoDequeue(&obj->tx,buffer,s);
	rtosUart[obj->uartId]->write((const char *)buffer,s);

	s = rtosUart[obj->uartId]->bytesAvailable();
	u32 max = getFifoFree(&obj->tx);
	if(s>max)
		s = max;
	if(s>10000)
		s = 10000;
	rtosUart[obj->uartId]->read((char *)buffer,s);
	fifoEnqueue(&obj->rx,buffer,s);

	return ISUCCESS;
}//uartFlush



