///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Uart.rpi.h
/// \brief		Gestion de l'UART
///
/// \author		Etienne BOILLEY
/// \date		21/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef UART_RPI_H
#define UART_RPI_H
#ifdef __cplusplus
extern "C" {
#endif



#ifndef RTOS_UART_NB
	#define RTOS_UART_NB 10
#endif



typedef const char * UartId;

typedef struct
{
	UartId				uartName;
	u16					uartId;

	Fifo				rx;  // FIFO réception UART
	Fifo				tx;  // FIFO émission UART

	Task				task;
}Uart;




#ifdef __cplusplus
}//extern C
#endif
#endif//UART_RPI_H
