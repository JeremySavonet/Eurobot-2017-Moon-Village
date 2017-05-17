///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Debug.h
/// \brief		Fonctions de débug standard
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		04/12/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef DEBUG_H
#define DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Fifo.h>



#ifndef RTOS_DEBUG
	#define RTOS_DEBUG					RTOS_DEBUG_ENABLE
#endif
#ifndef RTOS_DEBUG_ALWAYS_FLUSH
	#define RTOS_DEBUG_ALWAYS_FLUSH		0
#endif
#ifndef RTOS_DEBUG_INTERFACE_MAX
	#define RTOS_DEBUG_INTERFACE_MAX	3
#endif
#ifndef RTOS_DEBUG_PRINTF_SIZE
	#define RTOS_DEBUG_PRINTF_SIZE		500
#endif


#define RTOS_DEBUG_DISABLE	0		///< To set RTOS_DEBUG in Configuration.h
#define RTOS_DEBUG_ENABLE	1		///< To set RTOS_DEBUG in Configuration.h



#define rtosPrint(str,...)		debugPrint(DEBUG_COLOR_NORMAL str,##__VA_ARGS__)
#define rtosInfo(str,...)		debugPrint(DEBUG_COLOR_BLUE str,##__VA_ARGS__)
#define rtosSuccess(str,...)	debugPrint(DEBUG_COLOR_GREEN str,##__VA_ARGS__)
#define rtosWarning(str,...)	debugPrint(DEBUG_COLOR_YELLOW str,##__VA_ARGS__)
#define rtosError(str,...)		debugPrint(DEBUG_COLOR_RED str,##__VA_ARGS__)
#define rtosDebug(str,...)		debugPrint(str,##__VA_ARGS__)



// Définition des codes couleurs de la console vt100
#define DEBUG_COLOR_NORMAL		"\e[0m"
#define DEBUG_COLOR_BLUE		"\e[1;34m"
#define DEBUG_COLOR_GREEN		"\e[1;32m"
#define DEBUG_COLOR_RED			"\e[1;31m"
#define DEBUG_COLOR_YELLOW		"\e[1;33m"
#define DEBUG_COLOR_PURPLE		"\e[1;35m"
#define DEBUG_COLOR_BOLD		"\e[1m"
#define DEBUG_COLOR_UNDERLINE	"\e[4m"
#define DEBUG_COLOR_BLINK		"\e[5m"
#define DEBUG_COLOR_END			"\e[m"





#if RTOS_DEBUG==RTOS_DEBUG_ENABLE
	/// Initialise les fonctions de débug
	int debugInit( Fifo *fifo, Func1 flush, void *flushArg);

	/// Modifie la fifo de sortie
	int debugChangeOutput( Fifo *fifoToDelete, Fifo *fifo, Func1 flush, void *flushArg);

	/// Emet tout ce qui est contenu dans la fifo
	int debugFlush();

	/// Equivalent de printf
	int debugPrint( const char *format, ...);

#else
	/// Initialise les fonctions de débug
	#define debugInit(...)

	/// Modifie la fifo de sortie
	#define debugChangeOutput(...)

	/// Emet tout ce qui est contenu dans la fifo
	#define debugFlush(...)

	/// Equivalent de printf
	#define debugPrint(...)

#endif



#ifdef __cplusplus
}//extern C
#endif
#endif//DEBUG_H
