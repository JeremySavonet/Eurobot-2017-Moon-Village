// Copyright (c) 2016-2017 All Rights Reserved WestBot

//#define SIMU

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QThread>
#include <QTime>

#include <Defines.hpp>

#include <WestBot/Configuration.hpp>
#include <WestBot/ConfigurationTcpServer.hpp>
#include <WestBot/ExecuteMatch.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

#include <hps_arm.h> // For our base address


using namespace WestBot;

namespace
{
	WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );

	const int DEFAULT_PORT = 57497;
}

ExecuteMatch::ExecuteMatch()
	: _system( _hal )
{

}

void ExecuteMatch::run()
{

	qDebug() << "running";
//	while(1)
//		;


	//QCoreApplication app();

	// Load the configuration manager
/*	Configuration configurationManager( CONFIGURATION_PATH );

	QObject::connect(
		& configurationManager,
		& Configuration::configurationChanged,
		[ &configurationManager ]()
		{
			qDebug()
				<< QTime::currentTime().toString()
				<< "Update layers according to configuration changes";
			configurationManager.load();
		} );

	configurationManager.load();

	// Start the tcp configuration server
	ConfigurationTcpServer configurationServer( configurationManager );

	if( ! configurationServer.listen( QHostAddress::Any, DEFAULT_PORT ) )
	{
		qWarning()
			<< QTime::currentTime().toString()
			<< "Unable to start the server:"
			<< configurationServer.errorString();
	}

	configurationServer.showConnectionInformation();
*/
	// Init the Hardware Abstraction Layer
//	Hal hal;

	// Init the System manager
//	SystemManager system( _hal );

	// Init state machine and peripherals (sensors, position,...)
	if( ! _system.init() )
	{
		qWarning()
			<< QTime::currentTime().toString()
			<< "Failed to init system manager";
		return;
	}

#ifdef SIMU
	hal._modeSimu.write( 1 );
#endif

	// ========= Start the system =========


	qDebug() << "==== System ready ! ==== ";

	// ODOMETRY check
	int16_t x = _hal._odometryX.read< int16_t >();
	int16_t y = _hal._odometryY.read< int16_t >();
	int16_t theta = _hal._odometryTheta.read< int16_t >();

	qDebug()
		<< QTime::currentTime().toString()
		<< "X:" << x << " Y:" << y << " Theta:" << theta;

	// TODO: REWORK THIS
	int16_t safe = x + y + theta;

	if( safe > 0 )
	{
		while( 1 );
	}

	qDebug() << "System about to start";

	while(_system._startButton->digitalRead() ==  DigitalValue::OFF ) {
		QThread::msleep(20);
	}

	while(_system._startButton->digitalRead() !=  DigitalValue::OFF ) {
		QThread::msleep(20);
	}

	started = true;
	_system.start();

	//app.exec();
}


bool ExecuteMatch::isArretUrgence()
{
	if( _system._stopButton->digitalRead() == DigitalValue::ON )
		return true;
	return false;
}

void ExecuteMatch::hardStop()
{
	_system.hardStop();
}

void ExecuteMatch::funnyAction()
{
	_system.funnyAction();

}

void ExecuteMatch::displayLed()
{
	_system.displayColor( _system._colorButton->digitalRead() );

}
