///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Fifo.h
/// \brief		Buffers d'interconnexion des devices
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		16/05/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef FIFO_H
#define FIFO_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Common/Types.h>



/// Buffer permettant l'interconexion de devices
typedef struct
{
	volatile Size		debut;		///< index de début des données
	volatile Size		fin;		///< index de fin des données
	volatile Size		total;		///< nombre de caractères en file
	volatile Size		full;		///< 1 si plein
	volatile Size		max;		///< taille maximal
//	Size				sdata;		///< taille de la donnée
	u8				*data;		///< data
}Fifo;





int fifoInit( Fifo *f, u8 *data, Size size);			///< Initialise la fifo
int fifoReset( Fifo *f);								///< Vide la fifo

Size getFifoAvailable( Fifo *f);						///< Récupère l'espace occupé
Size getFifoFree( Fifo *f);								///< Récupère l'espace libre

int fifoEnqueueChar( Fifo *f, u8 data);					///< Ajoute un élément
int fifoEnqueue( Fifo *f, const void *array, Size size);///< Ajoute des éléments

int fifoPeekChar( Fifo *f);								///< Récupère un élément sans le supprimer
int fifoPeek( Fifo * f, void *data, Size size);			///< Récupère des éléments sans le supprimer

int fifoDequeueChar( Fifo *f);							///< Récupère un élément
int fifoDequeue( Fifo *f, void *data, Size size);		///< Récupère des éléments

int fifoDequeueQuick( Fifo *f, Size size);				///< Supprime des éléments



#ifdef __cplusplus
}//extern C
#endif
#endif // FIFO_H

