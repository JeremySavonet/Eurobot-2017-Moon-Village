///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Fifo.c
/// \brief		Buffers d'interconnexion des devices
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		16/05/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>



int fifoInit( Fifo *f, u8 *data, Size size)
{
	f->debut = 0;
	f->fin = 0;
	f->total = 0;
	f->full = 0;
	f->data = data;
	f->max = size;
	return ISUCCESS;
}//fifoInit

int fifoReset( Fifo *f)
{
	f->debut = 0;
	f->fin = 0;
	f->total = 0;
	f->full = 0;
	return ISUCCESS;
}//fifoReset



Size getFifoAvailable( Fifo *f)
{
//	Size debut = f->debut;
//	Size total;
//
//	if (debut <= f->fin)
//		total = f->fin - debut;
//	else
//		total = f->max - debut + f->fin;

//	f->total = total;

	return f->total;
}//getFifoAvaible

Size getFifoFree( Fifo *f)
{
//	Size debut = f->debut;
//	Size total;
//
//	if (debut <= f->fin)
//		total = f->fin - debut;
//	else
//		total = f->max - debut + f->fin;


	return f->max-f->total-1;
}//getFifoFree

int fifoEnqueueChar( Fifo *f, u8 data)
{
 	if( !(f->total+1 < f->max) )
		return IERROR;

	f->data[f->fin] = data;

	if (++f->fin == f->max)
		f->fin = 0;
	if (++f->total == f->max)
		f->full = 1;

	return ISUCCESS;
}//fifoEnqueueChar





int fifoEnqueue( Fifo *f, const void *array, Size size)
{
	Size s,l;

	Size debut = f->debut;
	Size total;

	if (debut <= f->fin)
		total = f->fin - debut;
	else
		total = f->max - debut + f->fin;

	f->total = total;

 	if( !(total + size < f->max && size != 0) )
		return IERROR;

	s = f->max - f->fin;
	if (size <= s)
		s = size;
	memcpy((char*)((char*)f->data+f->fin),array,s);
	if (s != size)
	{
		l = size-s;
		memcpy((char*)(f->data),(char*)((char*)array+s),l);
		f->fin = l;
	}
	else
		f->fin+=s;
	if (f->fin == f->max)
		f->fin = 0;

	f->total+=size;
	if (f->total == f->max)
		f->full = 1;

	return ISUCCESS;
}//fifoEnqueue



int fifoPeekChar( Fifo *f)
{
    // on vérifie que l'on a quelquechose en file
	if( !(f->total > 0) )
		return IERROR;
	
	return f->data[f->debut];
}//fifoPeekChar


int fifoPeek( Fifo * f, void *data, Size size)
{
	if( size==0 )
		return IBUSY;
	if( !(f->total >= size && size != 0) )
		return IERROR;

	Size s,l;

	s = f->max - f->debut;
	if (size <= s)
		s = size;
	memcpy(data,(char*)((char*)f->data+f->debut),s);

	if (s != size)
	{
		l = size-s;
		memcpy((char*)data+s,(char*)f->data,l);
	}

	return ISUCCESS;
}//fifoPeek






int fifoDequeueChar( Fifo *f)
{
	u8 result;

    // on vérifie que l'on a quelquechose en file
	if( !(f->total > 0) )
		return IERROR;
	
	result = f->data[f->debut];

	if (++f->debut == f->max) f->debut = 0;
	f->full = 0;
	f->total--;

	return result;
}//fifoDequeueChar



int fifoDequeue( Fifo *f, void *data, Size size)
{
	Size fin = f->fin;
	Size total;

	if (f->debut <= fin)
		total = fin - f->debut;
	else
		total = f->max - f->debut + fin;

	f->total = total;

	if( size==0 )
		return IBUSY;
	if( !(total >= size && size != 0) )
		return IERROR;

	Size s,l;
	s = f->max - f->debut;
	if (size <= s)
		s = size;
	memcpy(data,(char*)((char*)f->data+f->debut),s);

	if (s != size)
	{
		l = size-s;
		memcpy((char*)data+s,(char*)f->data,l);
		f->debut = l;
	}
	else
		f->debut+=s;

	if (f->debut == f->max)
		f->debut = 0;

	f->full = 0;
	f->total -= size;

	return ISUCCESS;
}//fifoDequeue



int fifoDequeueQuick( Fifo *f, Size size)
{
	if( size==0 )
		return IBUSY;
    // on vérifie que l'on a quelquechose en file
	if( !(f->total >= size && size != 0) )
		return IERROR;

	Size s,l;

	s = f->max - f->debut;
	if (size <= s) s = size;

	if (s != size)
	{
		l = size-s;
		f->debut = l;
	}
	else
		f->debut+=s;

	if (f->debut == f->max) f->debut = 0;

	f->full = 0;
	f->total -= size;

	return ISUCCESS;
}//fifoDequeueQuick



