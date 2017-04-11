// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define SIMU

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
    hal._congigL1.write( 1 );
#endif

    // Enable layer 1
    hal._stateL1.write( 1 );

    // Leds: Here just for an example
    Output led( std::make_shared< ItemRegister >( hal._output0 ), "Led" );
    Output io1( std::make_shared< ItemRegister >( hal._output1 ), "IO1" );

    // Activate output override for leds
    hal._outputOverride.write( 0x01010101 );

    StrategyManager strategyManager( system );

    // Carrousel TEST:
    hal._carrouselEnable.write( 0 );

    hal._carrouselOverride.write( 1 );

    hal._carrouselPidKp.write( (float)100.0 );
    hal._carrouselPidKi.write( (float)0.0 );
    hal._carrouselPidKd.write( (float)0.0 );
    hal._carrouselSpeed.write( (float)10.0 );
    hal._carrouselAcc.write( (float)0.001 );
    hal._carrouselOutputSaturation.write( 15000 );

    hal._carrouselTarget.write( hal._carrouselPosition.read<int32_t>() );
    hal._carrouselEnable.write(1);

    while( 1 )
    {
        led.digitalWrite( DigitalValue::ON );
        io1.digitalWrite( DigitalValue::OFF );
        QThread::msleep( 250 );
        led.digitalWrite( DigitalValue::OFF );
        io1.digitalWrite( DigitalValue::ON );
        QThread::msleep( 250 );
        QCoreApplication::processEvents();
    }

    return app.exec();
}
