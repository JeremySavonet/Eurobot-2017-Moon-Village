///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Rtos.rpi.h
/// \brief		Inclusion des header des fonctions du rtos
///
/// \author		Etienne BOILLEY
/// \date		16/04/2015
/// \copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RTOS_RPI_H
#define RTOS_RPI_H



	#include <stdio.h>
	#include <string.h>
	#include <stdarg.h>
	#include <stdlib.h>
	#include <math.h>
	#include <Rpi/Console.rpi.h>
	#include <Rpi/Time.rpi.h>


	typedef enum {
		RTOS_PRIORITY_LOW		= 10,
		RTOS_PRIORITY_NORMAL	= 20,
		RTOS_PRIORITY_HIGH		= 50,
		RTOS_PRIORITY_REAL_TIME	= 99
	}RtosPriority;


	int rtosInit();
	int rtosUseQtLoop();
	int rtosStart();
	int rtosExit();

	#define rtosDebugWelcome rtosDebugWelcome
	int rtosDebugWelcome();

	int rtosProcessorAffinityInit(int numberOfProcessorsToIsoalte);
	int rtosProcessorAffinitySet(int processor);
	int rtosProcessorAffinityIsolate();
	int rtosProcessorPriority( RtosPriority p);

#endif//RTOS_RPI_H
