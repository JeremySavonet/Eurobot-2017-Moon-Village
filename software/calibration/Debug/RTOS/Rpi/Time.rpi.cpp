///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Time.rpi.c
/// \brief		Gestion du temps pour le rtos
///
/// \author		Etienne BOILLEY
/// \date		20/09/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>
#include <QElapsedTimer>



QElapsedTimer rtosTime;



int rtosTimeInit( )
{
	rtosTime.start();
	rtosTime.restart();

	return ISUCCESS;
}//rtosTimeInit


Time getTime()
{
	Time t = rtosTime.nsecsElapsed();
	t /= 1000;
	return t;
}//getTime




