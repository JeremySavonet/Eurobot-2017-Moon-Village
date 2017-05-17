///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Debug.c
/// \brief		Fonctions de débug standard
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		04/12/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>

#include <stdarg.h>
#include <stdio.h>



#if RTOS_DEBUG==RTOS_DEBUG_ENABLE


	struct
	{
		Fifo	*fifo;
		Func1	flush;
		void	*flushArg;
	} rtosDebug[RTOS_DEBUG_INTERFACE_MAX] = {};



	int debugInit(	Fifo *fifo,		///< Fifo d'émission des messages
					Func1 flush,	///< Fonctions permettant de vider la fifo
					void *flushArg	///< Argument de la fonction
						)
	{
		if( fifo==NULL )
			return IERROR;

		u16 i;
		for( i=0; rtosDebug[i].fifo!=NULL && i<RTOS_DEBUG_INTERFACE_MAX; i++)
			;

		if( i>=RTOS_DEBUG_INTERFACE_MAX ) {
			rtosError("DEBUG\tNot enough interfaces\r\n");
			return IERROR;
		}

		rtosDebug[i].fifo		= fifo;
		rtosDebug[i].flush		= flush;
		rtosDebug[i].flushArg	= flushArg;

		rtosPrint("");

		return ISUCCESS;
	}//debugInit



	int debugChangeOutput(	Fifo *fifoToDelete,	///< Fifo à supprimer
							Fifo *fifo,			///< Fifo d'émission des messages
							Func1 flush,		///< Fonctions permettant de vider la fifo
							void *flushArg		///< Argument de la fonction
								)
	{
		if( fifo==NULL )
			return IERROR;

		u16 i;
		for( i=0; rtosDebug[i].fifo!=fifoToDelete && i<RTOS_DEBUG_INTERFACE_MAX; i++)
			;

		if( i>=RTOS_DEBUG_INTERFACE_MAX ) {
			rtosError("DEBUG\tFifo not found\r\n");
			return IERROR;
		}

		rtosWarning("DEBUG\tChange std output: %x to %x\r\n", fifoToDelete, fifo);
		rtosDebug[i].fifo		= fifo;
		rtosDebug[i].flush		= flush;
		rtosDebug[i].flushArg	= flushArg;

		rtosPrint( DEBUG_COLOR_NORMAL);

		return ISUCCESS;
	}//debugChangeOutput



	int debugPrint( const char *format, ...)
	{
		int r = ISUCCESS;
		char buf[RTOS_DEBUG_PRINTF_SIZE];
		s16 count;
		va_list args;

		va_start(args, format);
		count = vsnprintf (buf,sizeof(buf),format,args);
		va_end(args);

		if (count == -1)
			return IERROR;

		u16 i;
		for( i=0; rtosDebug[i].fifo!=NULL && i<RTOS_DEBUG_INTERFACE_MAX; i++)
		{
			if( fifoEnqueue( rtosDebug[i].fifo, buf, (Size)count) != ISUCCESS )
				r = IERROR;

			#if RTOS_DEBUG_ALWAYS_FLUSH
				debugFlush();
			#endif
		}
		return r;
	}//debug



	int debugFlush()
	{
		int r = ISUCCESS;
		u16 i;
		for( i=0; rtosDebug[i].fifo!=NULL && i<RTOS_DEBUG_INTERFACE_MAX; i++)
		{
			if( rtosDebug[i].flush != NULL )
				if( rtosDebug[i].flush(rtosDebug[i].flushArg) != ISUCCESS )
					r = IERROR;
		}
		return r;
	}//debugFlush



#endif//RTOS_DEBUG_ENABLE
