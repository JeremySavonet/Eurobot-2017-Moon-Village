///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Task.rpi.hpp
/// \brief		Gestion de l'appel des taches enregistrées par les devices
///
/// \author		Etienne BOILLEY
/// \date		02/12/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TASK_RPI_HPP
#define TASK_RPI_HPP


#include <QTimer>
#include <Rtos.h>



class RtosTask : public QObject
{
	Q_OBJECT

public:
	RtosTask( Func1 f, void *fArg, Time period, Time date) {
		this->f = f;
		this->fArg = fArg;
		this->period = period;
		this->date = date;
		connect( &timer, &QTimer::timeout, this, &RtosTask::task);
		if (date!=0)
			timer.setInterval(date/1000);
		else
			timer.setInterval(period/1000);
	}

	~RtosTask() {
		timer.stop();
	}

	QTimer	timer;

private slots:
	void task() {
		f(fArg);
		if (date!=0) {
			date = 0;
			timer.setInterval(period/1000);
		}
	}

private:
	Func1	f;
	void	*fArg;
	Time	period;
	Time	date;
};



#endif//TASK_RPI_HPP

