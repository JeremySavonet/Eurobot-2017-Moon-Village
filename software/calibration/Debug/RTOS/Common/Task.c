///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Task.c
/// \brief		Gestion de l'appel des taches enregistrées par les devices
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		30/10/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>



typedef enum
{
	RTOS_TASK_EMPTY = 0,
	RTOS_TASK_DISABLED,
	RTOS_TASK_ENABLED,
}RtosTaskStatus;
typedef struct
{
	Func1			f;				///< Fonction à exécuter
	void			*fArg;			///< Argument de la fonction
	Time			period;			///< Période d'exécution
	Time			date;			///< Date à laquelle excuter la fonction
	RtosTaskStatus	status;
}RtosTask;
RtosTask rtosTask[RTOS_TASK_NB] = {};



Task taskInit(	Func1	f,				///< Fonction à appeler
				void	*fArg,			///< Argument à donner à la fonction
				Time	period,			///< Période d'éxécution de la tache
				Time	date			///< Date à laquelle exécuter la tache
					)
{
	if( f==NULL)
		return IERROR;

	Task t;
	for( t=0; t<RTOS_TASK_NB; t++)
	{
		if( rtosTask[t].status==RTOS_TASK_EMPTY )
		{
			rtosTask[t].status = RTOS_TASK_DISABLED;
			rtosTask[t].f = f;
			rtosTask[t].fArg = fArg;
			rtosTask[t].period = period;
			rtosTask[t].date = date;
			return t;
		}
	}
	return IERROR;
}//taskInit



int taskDelete( Task t)
{
	if( (unsigned int)t>=RTOS_TASK_NB )
		return IERROR;

	taskStop( t);
	rtosTask[t].status = RTOS_TASK_EMPTY;
	return ISUCCESS;
}//taskDelete



int taskStart( Task t)
{
	if( (unsigned int)t>=RTOS_TASK_NB )
		return IERROR;
	if( rtosTask[t].status == RTOS_TASK_EMPTY )
		return IERROR;

	rtosTask[t].status = RTOS_TASK_ENABLED;
	return ISUCCESS;
}//taskStart



int taskStop( Task t)
{
	if( (unsigned int)t>=RTOS_TASK_NB )
		return IERROR;
	if( rtosTask[t].status == RTOS_TASK_EMPTY )
		return IERROR;

	rtosTask[t].status = RTOS_TASK_DISABLED;
	return ISUCCESS;
}//taskStop



int taskSetPeriod( Task t, Time period)
{
	if( (unsigned int)t>=RTOS_TASK_NB )
		return IERROR;
	if( rtosTask[t].status == RTOS_TASK_EMPTY )
		return IERROR;

	rtosTask[t].period = period;
	rtosTask[t].date = getTime() + period;
	return ISUCCESS;
}//taskSetPeriod



TaskStatus taskStatus( Task t)
{
	if( (unsigned int)t>=RTOS_TASK_NB )
		return TASK_ERROR;
	switch( rtosTask[t].status )
	{
		case RTOS_TASK_DISABLED :	return TASK_DISABLED;
		case RTOS_TASK_ENABLED :	return TASK_ENABLED;
		default :					return TASK_ERROR;
	}
}//taskStatus





int rtosTaskInit()
{
	Task t;
	for( t=0; t<RTOS_TASK_NB; t++)
	{
		rtosTask[t].status = RTOS_TASK_EMPTY;
	}

	return ISUCCESS;
}//rtosTaskInit



int rtosTaskExecute()
{
	Time time;
	Task t;

#if defined __dsPIC33F__ || defined __dsPIC33E__ || defined __STM32F7__
	while(1)
#endif
	{
		time = getTime();
		for( t=0; t<RTOS_TASK_NB; t++)
		{
			if( rtosTask[t].status != RTOS_TASK_ENABLED )
				continue;

			if( rtosTask[t].period == 0 )
			{
				if( rtosTask[t].f(rtosTask[t].fArg) < ISUCCESS )
				{
					rtosTask[t].status = RTOS_TASK_DISABLED;
					rtosError("TASK\tTask disabled\r\n");
				}
				continue;
			}

			if( rtosTask[t].date < time )
			{
				if( rtosTask[t].f(rtosTask[t].fArg) < ISUCCESS )
				{
					rtosTask[t].status = RTOS_TASK_DISABLED;
					rtosError("TASK\tTask disabled\r\n");
				}
				rtosTask[t].date = time + rtosTask[t].period;
				continue;
			}
		}
	}

	return ISUCCESS;
}//rtosTaskExecute

