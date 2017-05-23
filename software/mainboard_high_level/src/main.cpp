// Copyright (c) 2016-2017 All Rights Reserved WestBot

//#define SIMU

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QThread>

#include <Defines.hpp>

#include <WestBot/Configuration.hpp>
#include <WestBot/ConfigurationTcpServer.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

#include <hps_arm.h> // For our base address

namespace
{
    WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );

    const int DEFAULT_PORT = 57497;
}

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Load the configuration manager
    Configuration configurationManager( CONFIGURATION_PATH );

    QObject::connect(
        & configurationManager,
        & Configuration::configurationChanged,
        [ &configurationManager ]()
        {
            qDebug() << "Update layers according to configuration changes";
            configurationManager.load();
        } );

    configurationManager.load();

    // Start the tcp configuration server
    ConfigurationTcpServer configurationServer( configurationManager );

    if( ! configurationServer.listen( QHostAddress::Any, DEFAULT_PORT ) )
    {
        qWarning()
            << "Unable to start the server:"
            << configurationServer.errorString();
    }

    configurationServer.showConnectionInformation();

    // Init the Hardware Abstraction Layer
    Hal hal;

    // Init the System manager
    SystemManager system( hal );

    // Init state machine and peripherals (sensors, position,...)
    if( ! system.init() )
    {
        qWarning() << "Failed to init system manager";
        return EXIT_FAILURE;
    }

#ifdef SIMU
    hal._modeSimu.write( 1 );
#endif

    // ========= Start the system =========

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

    qDebug() << "==== System ready ! ==== ";

    // ODOMETRY check
    int16_t x = hal._odometryX.read< int16_t >();
    int16_t y = hal._odometryY.read< int16_t >();
    int16_t theta = hal._odometryTheta.read< int16_t >();

    qDebug() << "X:" << x << " Y:" << y << " Theta:" << theta;

    // TODO: REWORK THIS
    int16_t safe = x + y + theta;

    if( safe > 0 )
    {
        while( 1 );
    }

    return app.exec();
}
