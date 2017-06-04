///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Message.h
/// \brief		Messages échangés entre systèmes
///
/// \author		Etienne BOILLEY
/// \date		16/05/2016
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef MESSAGE_H
#define MESSAGE_H
#ifdef __cplusplus
extern "C" {
#endif


#include "../RTOS/Rtos.h"







///////////////////////////////////////////////////////////////////////////////////////////////////
///	Identifiants des messages
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MASK_DEST		0xF000				///< Les 4 premiers bits de l'id identifient la destination du message
#define MASK_CHANNEL	0x0F00				///< Les 4 suivants le cannal à utiliser (série, ethernet, spi...)
#define MASK_ID			0x00FF				///< Les 8 derniers identifient le contenu du message
	
typedef enum {
	ADDR_ID_ODOMETRY	= 0x0000,
	ADDR_ID_PID,							///< 
	ADDR_ID_CMD_POSITION,					///< Robot goto x y
	ADDR_ID_STATUS,
}MsgAddrId;
typedef enum {
	ADDR_CHANNEL_AUTO	= 0x0000,
}MsgAddrChannel;
typedef enum {
	ADDR_DEST_MATLAB	= 0x0000,
	ADDR_DEST_ARM		= 0x1000,
}MsgAddrDest;





///////////////////////////////////////////////////////////////////////////////////////////////////
/// Messages
///////////////////////////////////////////////////////////////////////////////////////////////////


typedef StructPacked {
	ProtocolHeader	header;
    uint32_t timestamp; 		// 1 = 20ns
	// 0 = Left, 1 = Right
    int32_t c_qei_sum[2]; 		// wheel encoder 
    int32_t m_qei_sum[2]; 		// motor encoder
    float x;
    float y;
    float teta_rad;
}MsgOdometry;

typedef StructPacked {
	ProtocolHeader	header;
    uint32_t timestamp; 		// 1 = 20ns
    float speed;
    float acc;
    float target;
    float filtered_target; 		// after quadramp
    float measure;
    float output;
}MsgPid;

typedef StructPacked {
	ProtocolHeader	header;
	f32		x;
	f32		y;
	f32		teta;
}MsgCmdPosition;

typedef StructPacked {
	ProtocolHeader	header;
	u8		progress;			// 100% => on est arrivé à la position, régu désactivé
	f32		x;
	f32		y;
	f32		teta;
}MsgStatus;




#ifdef __cplusplus
}//extern C
#endif
#endif//MESSAGE_H
