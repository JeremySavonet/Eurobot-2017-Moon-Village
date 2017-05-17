///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Configuration.h
/// \brief		Configuration du projet
///
/// \author		Etienne BOILLEY
/// \date		30/11/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CONFIGURATION_H
#define	CONFIGURATION_H


	#define PROJECT_NAME			"WESTBOT"
	#define PROJECT_VERSION			01
	#define PROJECT_REVISION		01

	#define RTOS_TASK_NB			25						///< Nombre de taches maximal
	#define RTOS_TASK_PERIOD		(0 MSEC)				///< Période d'exécution des task
	#define RTOS_PROGRAMM			RTOS_PROGRAMM_DEBUG		///< RTOS_PROGRAMM_DEBUG / RTOS_PROGRAMM_RELEASE
//	#define RTOS_DEBUG				RTOS_DEBUG_DISABLE		///< RTOS_DEBUG_ENABLE / RTOS_DEBUG_DISABLE
//	#define RTOS_DEBUG_ALWAYS_FLUSH	1
	#define RTOS_DEBUG_PRINTF_SIZE	5000					///< taille max d'un printf
	#define RTOS_LED_OK				17						///< Si cette led clignote, tout va bien. Si pas de pin dispo, utiliser pinDummy.
	#define RTOS_LED_ERR			27						///< Si cette led est allumée ou clignote, il y a un problème. Si pas de pin dispo, utiliser pinDummy.


//	#define PROTOCOL_SIZE_32BITS
//	#define PROTOCOL_MSG_SIZE_MAX							///< Taille maximale d'un message
	#define RTOS_PROTOCOL_RECV_ENTRY_MAX	10				///< Nombre de fonction de callback

	#define RTOS_DEBUG_PROTOCOL		3						///< Niveau de printf
	#define FILE_DEBUG				3						///< Niveau de printf


#endif//CONFIGURATION_H

