///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Console.rpi.h
/// \brief		Gestion de la console
///
/// \author		Etienne BOILLEY
/// \date		20/09/2015
/// \copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CONSOLE_RPI_H
#define CONSOLE_RPI_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Types.h>
#include <Common/Fifo.h>
#include <Common/Task.h>



#ifndef RTOS_CONSOLE_BUF_SIZE
	#define RTOS_CONSOLE_BUF_SIZE	50000
#endif

typedef struct {
	Fifo	f;
	u8		buffer[RTOS_CONSOLE_BUF_SIZE];
	Task	task;
}RtosConsole;
extern RtosConsole rtosConsole;


int consoleInit();	///< Initialisation de la console

int consoleFlush( void *dummy);				///< Envoi tout ce qui est contenu dans la fifo



#ifdef __cplusplus
}//extern C
#endif
#endif//CONSOLE_RPI_H
