#ifndef EXECUTEMATCH_H
#define EXECUTEMATCH_H

#include <QThread>
#include <QObject>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

namespace WestBot {

class ExecuteMatch : public QThread
{
	Q_OBJECT

	void run();

public:
	ExecuteMatch();

	bool isArretUrgence();
	void hardStop();
	void funnyAction();
	void displayLed();

	bool started = false;

private:
	Hal _hal;
	SystemManager _system;

//signals:
//	void started();

};

}

#endif // EXECUTEMATCH_H
