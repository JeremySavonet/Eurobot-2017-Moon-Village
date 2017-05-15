///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Protocol.h
/// \brief		Système d'échanges de messages entre processeurs
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		30/10/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef PROTOCOL_H
#define PROTOCOL_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Types.h>
#include <Common/Fifo.h>


//Configuration
	#ifndef RTOS_PROTOCOL_RECV_FIFO_MAX
		#define RTOS_PROTOCOL_RECV_FIFO_MAX		5
	#endif
	#ifndef RTOS_PROTOCOL_RECV_ENTRY_MAX
		#define RTOS_PROTOCOL_RECV_ENTRY_MAX	5
	#endif
	#ifndef RTOS_PROTOCOL_SEND_ENTRY_MAX
		#define RTOS_PROTOCOL_SEND_ENTRY_MAX	5
	#endif

	#ifndef PROTOCOL_MSG_SIZE_MAX
		#define PROTOCOL_MSG_SIZE_MAX			100
	#endif
	
	
//Header
	#ifndef PROTOCOL_SIZE_32BITS
		typedef u16 ProtocolSize;
	#else
		typedef u32 ProtocolSize;
	#endif
	#define PROTOCOL_FANION		0xA5
	typedef StructPacked {
		u8		fanion;
		ProtocolSize	size;
		u16		crc;
		u16		id;
	}ProtocolHeader;

	
//Messages avec nom des champs (convertibles en requetes SQL, CSV...)
	//Exemple:
	//#define MSG(msgStruct) msgStruct( name=test, sqlPrimaryField=time,
	//			time,		u64,	FIELD_NORMAL,
	//			pm2,		u32,	FIELD_NORMAL,
	//			pm10,		u32,	FIELD_NORMAL,
	//			co2,		f32,	FIELD_NORMAL,
	//			humidity,	f32,	FIELD_NORMAL,
	//			windSpeed,	f32,	FIELD_NORMAL,
	//			windDir,	f32,	FIELD_NORMAL )
	//MSG( MSG_STRUCT_C );
	//MSG( MSG_STRUCT_WITH_FIELD_NAME );

	//struct type C
		#define MSG_STRUCT_C( name, primary, ...) \
			typedef StructPacked { \
				ProtocolHeader header; \
				MACRO_FOREACH_3ARG( MSG_STRUCT_C_FIELD, __VA_ARGS__) \
			}Msg##name
		#define MSG_STRUCT_C_FIELD(name,type,...) type name;

	//struct type SQL, CSV...
		#define getMsgWithFieldName(name)	&msgWithFieldName##name
		#define MSG_STRUCT_WITH_FIELD_NAME( name, primary, ...) \
			static const struct  \
			{ \
				const char	*name; \
				const char	*primary; \
				Field		field[MACRO_DEC(MACRO_DEC(MACRO_NARG(__VA_ARGS__)))/3+2]; \
			}msgWithFieldName##name = \
			{ \
				#name, \
				#primary, \
				{ MACRO_FOREACH_3ARG( MSG_STRUCT_WITH_FIELD_NAME_FIELD, __VA_ARGS__, end,u8,FIELD_END ) } \
			}
		#define MSG_STRUCT_WITH_FIELD_NAME_FIELD(name,type,property) {#name,#type,property},
		
		typedef enum
		{
			FIELD_END = 0,
			FIELD_NORMAL,
			FIELD_AUTOINC
		}FieldProperty;
		typedef struct
		{
			const char		*name;
			const char		*type;
			FieldProperty property;
		}Field;
		typedef struct
		{
			const char	*name;
			const char	*primary;
			Field		field[1];
		}MsgWithFieldName;


	//Fonctions
		int protocolInit();			///<

		int protocolAddRecvFifo( Fifo *fifo );	///<
		int protocolAddRecvMsg( u16 addr, u16 mask, Func2 cb, void *cbArg );	///<

		int protocolAddSendFifo( u16 addr, u16 mask, Fifo *fifo );	///<

		int sendMsg( u16 addr, void *msg, ProtocolSize size); ///<

		u16 protocolCrc( u8 *msg, ProtocolSize size);



#ifdef __cplusplus
}//extern C
#endif
#endif//PROTOCOL_H
