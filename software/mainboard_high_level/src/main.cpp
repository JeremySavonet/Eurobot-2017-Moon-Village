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

    TrajectoryManager trajectoryManager( hal );
    trajectoryManager.init();

    qDebug() << "Platform ready...";

#ifdef SIMU 
    hal._modeSimu.write( 1 );
#endif

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

    // Test trajectory module
    uint8_t inWindow;
    uint8_t commandId = hal._trajOutAck.read< uint8_t >();

    hal._trajCmdValid.write(0x0);
    hal._trajCmdId.write( commandId++ );
    hal._trajCmdType.write( CMD_TYPE_TRAJ );
    hal._trajCmdOrderType.write( TRAJ_GOTO_XY_ABS );
    hal._trajCmdPosTheta.write( ( float ) 0.0 );
    hal._trajCmdPosX.write( ( float ) 1200.0 );
    hal._trajCmdPosY.write( ( float ) 200.0 );

    hal._trajCmdValid.write( 0x1) ;

    while( hal._trajOutAck.read< uint8_t >() != commandId )
    {
       QThread::msleep( 10 );
       qDebug() << "wait cmd ack";
    }
    do
    {
       QThread::msleep( 100 );
       state = static_cast< TrajectoryManager::TrajectoryState >( hal._trajOutState.read< uint8_t >() );
       inWindow = hal._trajOutInWindow.read< uint8_t >();
       qDebug() << "wait traj ready ....";
    } while( inWindow == 0 );

    while( 1 )
    {
        QThread::msleep( 250 );
        QCoreApplication::processEvents();
    }

    return app.exec();
}
