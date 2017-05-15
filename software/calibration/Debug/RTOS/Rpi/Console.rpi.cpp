///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Console.rpi.cpp
/// \brief		Gestion de la console
///
/// \author		Etienne BOILLEY
/// \date		20/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>



RtosConsole rtosConsole;

int consoleTask( void *dummy);



int consoleInit()
{
	memset( &rtosConsole, 0, sizeof(rtosConsole));

	fifoInit( &rtosConsole.f, rtosConsole.buffer, RTOS_CONSOLE_BUF_SIZE);

	#if !RTOS_DEBUG_ALWAYS_FLUSH
		rtosConsole.task = taskInit( (Func1)consoleTask, NULL, 10 MSEC, 0);
		taskStart( rtosConsole.task);
	#endif

	return ISUCCESS;
}//consoleInit


int consoleTask( void *dummy)
{
	ignoreArg(dummy);
	consoleFlush(NULL);
	return ISUCCESS;
}//consoleTask


int consoleFlush( void *dummy)
{
	ignoreArg(dummy);
	u8 buffer[RTOS_CONSOLE_BUF_SIZE+1];
	int s = getFifoAvailable(&rtosConsole.f);
	if (s>RTOS_CONSOLE_BUF_SIZE)
		s = RTOS_CONSOLE_BUF_SIZE;
	fifoDequeue(&rtosConsole.f,buffer,s);
	buffer[s] = 0;
	fprintf(stderr,(const char *)buffer);
	return ISUCCESS;
}//consoleFlush


