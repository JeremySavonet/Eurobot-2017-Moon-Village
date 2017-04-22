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
#include <WestBot/Servo.hpp>
#include <WestBot/StrategyManager.hpp>
#include <WestBot/SystemManager.hpp>

#define SERVO_0_ARM_R_CLOSED 31217
#define SERVO_0_ARM_R_OPEN 15590
#define SERVO_0_ARM_R_OPEN90 20000
#define SERVO_0_ARM_R_DROP 30275

#define SERVO_6_ARM_L_CLOSED 12713
#define SERVO_6_ARM_L_OPEN 31217
#define SERVO_6_ARM_L_OPEN90 23000
#define SERVO_6_ARM_L_DROP 14155

#define SERVO_7_EJECTOR_STANDBY 31217
#define SERVO_7_EJECTOR_EJECT 11533

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

    Servo s0( "Arm_right" );
    s0.attach( hal, 0, SERVO_0_ARM_R_OPEN90, SERVO_0_ARM_R_CLOSED );

    if( ! s0.isAttached() )
    {
        qWarning() << "Failed to attached servo arm right...";
        return EXIT_FAILURE;
    }

    Servo s6( "Arm_left" );
    s6.attach( hal, 6, SERVO_6_ARM_L_OPEN90, SERVO_6_ARM_L_CLOSED );

    if( ! s6.isAttached() )
    {
        qWarning() << "Failed to attached servo arm left...";
        return EXIT_FAILURE;
    }

    Servo s7( "Ejector" );
    s7.attach( hal, 7, SERVO_7_EJECTOR_STANDBY, SERVO_7_EJECTOR_EJECT );

    if( ! s7.isAttached() )
    {
        qWarning() << "Failed to attached servo ejector...";
        return EXIT_FAILURE;
    }

    carrousel.setPosition( 1.0 );

    // TEST SERVO
    while( 1 )
    {
        s0.write( SERVO_0_ARM_R_CLOSED );
        s6.write( SERVO_6_ARM_L_CLOSED );

        QThread::msleep( 1000 );

        s0.write( SERVO_0_ARM_R_DROP );
        s6.write( SERVO_6_ARM_L_DROP );

        QThread::msleep( 1000 );

        s7.write( SERVO_7_EJECTOR_EJECT );

        QThread::msleep( 1000 );

        s0.write( SERVO_0_ARM_R_OPEN90 );
        s6.write( SERVO_6_ARM_L_OPEN90 );

        QThread::msleep( 1000 );

        s7.write( SERVO_7_EJECTOR_STANDBY );

        QThread::msleep( 5000 );

        s0.disable();
        s6.disable();
        s7.disable();

        QThread::msleep( 5000 );

        s0.enable();
        s6.enable();
        s7.enable();

        QCoreApplication::processEvents();
    }

    return app.exec();
}
