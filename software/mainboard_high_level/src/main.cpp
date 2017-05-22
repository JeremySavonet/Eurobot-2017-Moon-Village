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

    Carrousel carrousel( hal );

    if( ! carrousel.init() )
    {
        qWarning() << "Failed to init carrousel module...";
        return EXIT_FAILURE;
    }

    // Set carrousel offset
    carrousel.setPosition( 1.05 );

    // ========= Start the system =========

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

    qDebug() << "Platform ready...";

    TrajectoryManager trajectoryManager( hal );
    trajectoryManager.init();

    StrategyManager strategyManager(
        system,
        trajectoryManager,
        carrousel,
        s0,
        s6,
        s7,
        turbine );

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
