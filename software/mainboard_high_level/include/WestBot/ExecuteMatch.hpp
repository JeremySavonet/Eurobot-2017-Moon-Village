// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_EXECUTEMATCH_HPP
#define WESTBOT_EXECUTEMATCH_HPP

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
};

}

#endif // WESTBOT_EXECUTEMATCH_HPP
