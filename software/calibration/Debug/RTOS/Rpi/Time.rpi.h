///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Time.rpi.h
/// \brief		Gestion du temps pour le rtos
///
/// \author		Etienne BOILLEY
/// \date		20/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TIME_RPI_H
#define TIME_RPI_H
#ifdef __cplusplus
extern "C" {
#endif


#include <Common/Types.h>



#define USEC	*((Time)1)
#define MSEC	*((Time)1000 USEC)
#define SEC		*((Time)1000 MSEC)
#define MIN		*((Time)60 SEC)
#define HOUR	*((Time)60 MIN)
#define DAY		*((Time)24 HOUR)

#ifndef delay
	#define delay(us)	QThread::usleep(us)
#endif


int rtosTimeInit();				///< Initialise le système de gestion du temps

Time getTime();					///< Retourne le temps en µs depuis le boot



#ifdef __cplusplus
}//extern C
#endif
#endif//TIME_RPI_H
