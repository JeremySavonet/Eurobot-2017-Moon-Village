///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Task.h
/// \brief		Gestion de l'appel des taches enregistrées par les devices
///
/// \author		Etienne BOILLEY
/// \date		20/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TASK_H
#define TASK_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Types.h>



#ifndef RTOS_TASK_NB
	#define RTOS_TASK_NB 25
#endif


#ifdef __QT__
	typedef void* Task;
#else
	typedef int Task;
#endif

typedef enum
{
	TASK_ENABLED = 0,
	TASK_DISABLED = 1,
	TASK_ERROR = IERROR,
}TaskStatus;


Task taskInit( Func1 callback, void *arg, Time period, Time date);	///< Enregistre une nouvelle tache
int taskDelete( Task t);											///< Supprime une tache

int taskStart( Task t);												///< Démarre la tache
int taskStop( Task t);												///< Démarre la tache
int taskSetPeriod( Task t, Time period);							///< Change la période d'exécution
TaskStatus taskStatus( Task t);										///< Récupère l'état de la tache

int rtosTaskInit();													///< Initialise le système de taches utilisateur
int rtosTaskExecute();												///< Fonction exécutant toutes les taches enregistrées




#ifdef __cplusplus
}//extern C
#endif
#endif//TASK_H
