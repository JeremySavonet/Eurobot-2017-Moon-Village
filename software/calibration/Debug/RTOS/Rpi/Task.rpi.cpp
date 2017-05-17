///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Task.crpi.pp
/// \brief		Gestion de l'appel des taches enregistrées par les devices
///
/// \author		Etienne BOILLEY
/// \date		02/12/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>
#include "Task.rpi.hpp"




Task taskInit(	Func1	f,				///< Fonction à appeler
				void	*fArg,			///< Argument à donner à la fonction
				Time	period,			///< Période d'éxécution de la tache
				Time	date			///< Date à laquelle exécuter la tache
					)
{
	if( f==NULL)
		return NULL;

	RtosTask *t = new RtosTask( f, fArg, period, date);
	return (void*)t;
}//taskInit



int taskDelete( Task t)
{
	if( t==NULL )
		return IERROR;

	((RtosTask*)t)->~RtosTask();

	return ISUCCESS;
}//taskDelete



int taskStart( Task t)
{
	if( t==NULL )
		return IERROR;

	((RtosTask*)t)->timer.start();
	return ISUCCESS;
}//taskStart



int taskStop( Task t)
{
	if( t==NULL )
		return IERROR;

	((RtosTask*)t)->timer.stop();
	return ISUCCESS;
}//taskStop



int taskSetPeriod( Task t, Time period)
{
	if( t==NULL )
		return IERROR;

	((RtosTask*)t)->timer.setInterval(period/1000);
	return ISUCCESS;
}//taskSetPeriod



TaskStatus taskStatus( Task t)
{
	if( t==NULL )
		return TASK_ERROR;

	if( ((RtosTask*)t)->timer.isActive() )
		return TASK_ENABLED;
	else
		return TASK_DISABLED;
}//taskStatus




