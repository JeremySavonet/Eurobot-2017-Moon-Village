// Copyright (c) 2016-2017 All Rights Reserved WestBot

//#define SIMU

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QThread>

#include <Defines.hpp>

#include <WestBot/Carrousel.hpp>
#include <WestBot/Configuration.hpp>
#include <WestBot/ConfigurationTcpServer.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/Output.hpp>
#include <WestBot/StrategyManager.hpp>
#include <WestBot/SystemManager.hpp>

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
    hal.init(); // Open the FPGA memory and load our module

    // Init the System manager
    SystemManager system( hal );
    system.init(); // Create the state machine

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

#ifdef SIMU 
    hal._modeSimu.write( 1 );
#endif

    // Activate output override for leds
    hal._outputOverride.write( 0x01010101 );

    StrategyManager strategyManager( system );

    Carrousel carrousel( hal );

    if( ! carrousel.init() )
    {
        qWarning() << "Failed to init carrousel module...";
        return EXIT_FAILURE;
    }

    float pos = 6.0;
    uint8_t id = 0;
    while( 1 )
    {
        id++;
        QThread::msleep( 1000 );
        carrousel.setPosition( pos );
        if( id <= 30 )
        {
            pos -= 0.5f;
        }
        if( id >= 30 && id <= 60 )
        {
            pos += 0.5f;
        }

        if( pos < 0.0 )
        {
            pos += 6.0;
        }

        if( pos > 6.0 )
        {
            pos -= 6.0;
        }

        QCoreApplication::processEvents();
    }

    return app.exec();
}
