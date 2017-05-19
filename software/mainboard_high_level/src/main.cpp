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
#include <WestBot/TrajectoryManager.hpp>
#include <WestBot/Turbine.hpp>

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
    hal.init(); // Open the FPGA memory and load our module

    hal._outputOverride.write( 0x01010101 );

    // Init the System manager
    SystemManager system( hal );
    system.init(); // Create the state machine
    system.setMode( SystemManager::SystemMode::Full ); // Set the system in full mode

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

    TrajectoryManager trajectoryManager( hal );
    trajectoryManager.init();

    qDebug() << "Platform ready...";

#ifdef SIMU
    hal._modeSimu.write( 1 );
#endif

    Carrousel carrousel( hal );

    if( ! carrousel.init() )
    {
        qWarning() << "Failed to init carrousel module...";
        return EXIT_FAILURE;
    }

    // Set carrousel offset
    carrousel.setPosition( 1.05 );

    Servo s0( "Arm_right" );
    s0.attach( hal, 0, SERVO_0_ARM_R_OPEN90, SERVO_0_ARM_R_CLOSED );

    if( ! s0.isAttached() )
    {
        qWarning() << "Failed to attach servo arm right...";
        return EXIT_FAILURE;
    }

    Servo s6( "Arm_left" );
    s6.attach( hal, 6, SERVO_6_ARM_L_OPEN90, SERVO_6_ARM_L_CLOSED );

    if( ! s6.isAttached() )
    {
        qWarning() << "Failed to attach servo arm left...";
        return EXIT_FAILURE;
    }

    Servo s7( "Ejector" );
    s7.attach( hal, 7, SERVO_7_EJECTOR_STANDBY, SERVO_7_EJECTOR_EJECT );

    if( ! s7.isAttached() )
    {
        qWarning() << "Failed to attach servo ejector...";
        return EXIT_FAILURE;
    }

    Turbine turbine( "Turbine" );
    if( ! turbine.attach( hal ) )
    {
        qWarning() << "Failed to attach turbine...";
        return EXIT_FAILURE;
    }

    StrategyManager strategyManager(
        system,
        trajectoryManager,
        carrousel,
        s0,
        s6,
        s7,
        turbine );

    qDebug() << "==== System ready ! ==== ";

    return app.exec();
}
