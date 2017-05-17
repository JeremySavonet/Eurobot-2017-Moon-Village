///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Uart.h
/// \brief		Gestion de l'UART
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		27/10/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef UART_H
#define UART_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Fifo.h>
#include <Common/Task.h>

#if defined __dsPIC33F__ || defined __dsPIC33E__
	#include <Pic/Uart.pic.h>
#elif defined(__STM32F745xx__) || defined(__STM32F746xx__) || defined(__STM32F765xx__)
	#include <Stm/Uart.stm.h>
#elif defined __QT__
	#include <Rpi/Uart.rpi.h>
#else
	#error Processor not supported
#endif




typedef enum
{
	STM32_BAUDRATE_,				///< all<25Mbps
}Stm32Baudrate;
typedef enum
{
	RPI_BAUDRATE_,
}RpiBaudrate;						///< all<4Mbps
typedef enum
{
	DSPIC33E_BAUDRATE_16_833000 = 833000,
	DSPIC33E_BAUDRATE_16_1250000 = 1250000,
	DSPIC33E_BAUDRATE_16_2500000 = 2500000,
}Dspic33eBaudrate16;				///< 40e6 /16 /n
typedef enum
{
	DSPIC33E_BAUDRATE_4_833000 = 833000,
	DSPIC33E_BAUDRATE_4_909000 = 909000,
	DSPIC33E_BAUDRATE_4_1000000 = 1000000,
	DSPIC33E_BAUDRATE_4_1111000 = 1111000,
	DSPIC33E_BAUDRATE_4_1250000 = 1250000,
	DSPIC33E_BAUDRATE_4_1429000 = 1429000,
	DSPIC33E_BAUDRATE_4_1667000 = 1667000,
	DSPIC33E_BAUDRATE_4_2000000 = 2000000,
	DSPIC33E_BAUDRATE_4_2500000 = 2500000,
	DSPIC33E_BAUDRATE_4_3333000 = 3333000,
	DSPIC33E_BAUDRATE_4_5000000 = 5000000,
	DSPIC33E_BAUDRATE_4_10000000 = 10000000,
}Dspic33eBaudrate4;					///< 40e6 /4 /n
typedef enum
{
	US232_BAUDRATE_828000 = 828000,
	US232_BAUDRATE_857000 = 857000,
	US232_BAUDRATE_889000 = 889000,
	US232_BAUDRATE_923000 = 923000,
	US232_BAUDRATE_960000 = 960000,
	US232_BAUDRATE_1000000 = 1000000,
	US232_BAUDRATE_1043000 = 1043000,
	US232_BAUDRATE_1091000 = 1091000,
	US232_BAUDRATE_1143000 = 1143000,
	US232_BAUDRATE_1200000 = 1200000,
	US232_BAUDRATE_1263000 = 1263000,
	US232_BAUDRATE_1333000 = 1333000,
	US232_BAUDRATE_1412000 = 1412000,
	US232_BAUDRATE_1500000 = 1500000,
	US232_BAUDRATE_2000000 = 2000000,
	US232_BAUDRATE_3000000 = 3000000,
}Us232Baudrate;					///<  24e6 /[34 30 26 25 24...]
typedef enum
{
	CH341_BAUDRATE_750000 = 750000,
	CH341_BAUDRATE_857000 = 857000,
	CH341_BAUDRATE_1000000 = 1000000,
	CH341_BAUDRATE_1200000 = 1200000,
	CH341_BAUDRATE_1500000 = 1500000,
	CH341_BAUDRATE_2000000 = 2000000,
	CH341_BAUDRATE_3000000 = 3000000,
}Ch341Baudrate;					///< 6e6 /n
typedef enum
{
	ATEN_BAUDRATE_820000 = 820000,
	ATEN_BAUDRATE_923000 = 923000,
	ATEN_BAUDRATE_1200000 = 1200000,
	ATEN_BAUDRATE_2500000 = 2500000,
	ATEN_BAUDRATE_3000000 = 3000000,
}AtenBaudrate;





int uartInit( Uart *obj, UartId id, u32 baudrate, u8 rx[], Size rxSize, u8 tx[], Size txSize);	///< Initialisation de l'UART
int uartSetBrg( Uart *obj, u32 baudrate);

// Envoi tout ce qui est contenu dans la fifo
int uartFlush( Uart *obj);



#ifdef __cplusplus
}//extern C
#endif
#endif//UART_H
