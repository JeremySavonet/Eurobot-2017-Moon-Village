///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Rtos.h
/// \brief		Inclusion des header des fonctions du rtos
///
/// \author		Etienne BOILLEY
/// \date		16/04/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RTOS_H
#define RTOS_H



    #include <Configuration.h>

	#define RTOS_PROGRAMM_RELEASE	0			///< To set RTOS_PROGRAMM in configuration.h
	#define RTOS_PROGRAMM_DEBUG		1			///< To set RTOS_PROGRAMM in configuration.h
	#ifndef RTOS_PROGRAMM
		#define RTOS_PROGRAMM RTOS_PROGRAMM_DEBUG
	#endif
	#ifndef RTOS_TASK_PERIOD
		#define RTOS_TASK_PERIOD 0
	#endif

	#include <Common/Macro.h>
	#include <Common/Types.h>
	#include <Common/Fifo.h>
	#include <Common/Debug.h>
	#include <Common/Task.h>
	#include <Common/Uart.h>
	#include <Common/Protocol.h>
//	#include <Common/Thread.h>


	#if defined __dsPIC33F__ || defined __dsPIC33E__
		#include <Common/Utils.h>
		#include <Pic/Rtos.pic.h>
	#elif defined __STM32F7__
		#include <Stm/Rtos.stm.h>
	#elif defined __QT__
		#include <Rpi/Rtos.rpi.h>
		#include <Rpi/Console.rpi.h>
	#else
		#error Processor not supported
	#endif



#endif//RTOS_H
