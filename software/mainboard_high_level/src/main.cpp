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

    StrategyManager strategyManager(
        system,
        trajectoryManager,
        carrousel,
        s0,
        s6,
        s7 );

    MemoryManager memoryManager;

    // TEST PURPOSE
    // Create our layer mapping
    Memory layer2( memoryManager, PIO_N_LAYER2_BASE );
    //Memory layer3( memoryManager, PIO_N_LAYER3_BASE );

    // PID D
    ItemRegister pid_distance_en( layer2, 16 * 4, 8 );
    ItemRegister pid_distance_override( layer2, 16 * 4+1, 8 );
    ItemRegister pid_distance_inverted( layer2, 16 * 4+2, 8 );

    ItemRegister pid_distance_p( layer2, 17 * 4, 32 );
    ItemRegister pid_distance_i( layer2, 18 * 4, 32 );
    ItemRegister pid_distance_d( layer2, 19 * 4, 32 );
    ItemRegister pid_distance_speed( layer2,20 * 4, 32 );
    ItemRegister pid_distance_acc( layer2, 21 * 4, 32 );
    ItemRegister pid_distance_sat( layer2, 22 * 4, 32 );

    ItemRegister pid_distance_pos( layer2, 23 * 4, 32 );
    ItemRegister pid_distance_target( layer2, 24 * 4, 32 );

    pid_distance_en.write(0);
    pid_distance_override.write(0);
    pid_distance_inverted.write(0);
    pid_distance_p.write((float)2000.0);
    pid_distance_i.write((float)0.0);
    pid_distance_d.write((float)0.0);
    pid_distance_speed.write((float)0.01);
    pid_distance_acc.write((float)0.0001);
    pid_distance_sat.write(25000);

    pid_distance_target.write(pid_distance_pos.read<float>());
    pid_distance_en.write(1);
    pid_distance_target.write(pid_distance_pos.read<float>());

    // PID A
    ItemRegister pid_angle_en( layer2, 27 * 4, 8 );
    ItemRegister pid_angle_override( layer2, 27 * 4+1, 8 );
    ItemRegister pid_angle_inverted( layer2, 27 * 4+2, 8 );

    ItemRegister pid_angle_p( layer2, 28 * 4, 32 );
    ItemRegister pid_angle_i( layer2, 29 * 4, 32 );
    ItemRegister pid_angle_d( layer2, 30 * 4, 32 );
    ItemRegister pid_angle_speed( layer2,31 * 4, 32 );
    ItemRegister pid_angle_acc( layer2, 32 * 4, 32 );
    ItemRegister pid_angle_sat( layer2, 33 * 4, 32 );

    ItemRegister pid_angle_pos( layer2, 34 * 4, 32 );
    ItemRegister pid_angle_target( layer2, 35 * 4, 32 );

    pid_angle_en.write(0);
    pid_angle_override.write(0);
    pid_angle_inverted.write(1);
    pid_angle_p.write((float)500000.0);

    pid_angle_i.write((float)0.0);
    pid_angle_d.write((float)0.0);
    pid_angle_speed.write((float)0.0001);
    pid_angle_acc.write((float)0.00000002);
    pid_angle_sat.write(25000);

    pid_angle_target.write(pid_angle_pos.read<float>());
    pid_angle_en.write(1);

//#define CMD_TYPE_TRAJ 0x0
//#define CMD_TYPE_CFG_DISTANCE 0x1
//#define CMD_TYPE_CFG_ANGLE 0x2
//#define CMD_TYPE_CFG_WND 0x3

//#define TRAJ_DISABLE 0
//#define TRAJ_ENABLE 1
//#define TRAJ_STOP 2
//#define TRAJ_HARDSTOP 3
//#define TRAJ_D_REL 4
//#define TRAJ_ONLY_D_REL 5
//#define TRAJ_A_REL 6
//#define TRAJ_A_ABS 7
//#define TRAJ_ONLY_A_REL 8
//#define TRAJ_ONLY_A_ABS 9
//#define TRAJ_D_A_REL 10
//#define TRAJ_TURNTO_XY 11
//#define TRAJ_TURNTO_XY_BEHIND 12
//#define TRAJ_GOTO_XY_ABS 13
//#define TRAJ_GOTO_FORWARD_XY_ABS 14
//#define TRAJ_GOTO_BACKWARD_XY_ABS 15
//#define TRAJ_GOTO_D_A_REL 16
//#define TRAJ_GOTO_XY_REL 17

//    ItemRegister traj_freq_hz( layer3, 2 * 4, 16 );
//    traj_freq_hz.write(10);

//    ItemRegister traj_cmd_valid( layer3, 11 * 4, 8 );
//    ItemRegister traj_cmd_id( layer3, 11 * 4+1, 8 );
//    ItemRegister traj_cmd_type( layer3, 11 * 4+2, 8 );
//    ItemRegister traj_cmd_order_type( layer3, 11 * 4+3, 8 );

//    ItemRegister traj_cmd_wnd_distance( layer3, 12 * 4, 32 );
//    ItemRegister traj_cmd_wnd_angle_deg( layer3, 13 * 4, 32 );
//    ItemRegister traj_cmd_wnd_angle_start_deg( layer3, 14 * 4, 32 );

//    ItemRegister traj_cmd_cfg_speed( layer3, 12 * 4, 32 );
//    ItemRegister traj_cmd_cfg_acc( layer3, 13 * 4, 32 );

//    uint8_t cmd_id = traj_out_ack.read<uint8_t>();

//    traj_cmd_valid.write(0x0);
//    traj_cmd_id.write(cmd_id++);
//    traj_cmd_type.write(CMD_TYPE_TRAJ);
//    traj_cmd_order_type.write(TRAJ_DISABLE);
//    traj_cmd_valid.write(0x1);

//    while (traj_out_ack.read<uint8_t>() != cmd_id)
//    {
//        QThread::msleep( 10 );
//        qDebug() << "wait cmd ack";
//    }

//    traj_cmd_valid.write(0x0);
//    traj_cmd_id.write(cmd_id++);
//    traj_cmd_type.write(CMD_TYPE_CFG_WND);
//    traj_cmd_wnd_distance.write((float)10.0);
//    traj_cmd_wnd_angle_deg.write((float)2.0);
//    traj_cmd_wnd_angle_start_deg.write((float)10.0);
//    traj_cmd_valid.write(0x1);

//    while (traj_out_ack.read<uint8_t>() != cmd_id)
//    {
//        QThread::msleep( 2000 );
//        qDebug() << "wait cmd ack " << traj_out_ack.read<uint8_t>();
//    }

//    traj_cmd_valid.write(0x0);
//    traj_cmd_id.write(cmd_id++);
//    traj_cmd_type.write(CMD_TYPE_CFG_DISTANCE);
//    traj_cmd_cfg_speed.write((float)0.12);
//    traj_cmd_cfg_acc.write((float)0.00001);
//    traj_cmd_valid.write(0x1);

//    while (traj_out_ack.read<uint8_t>() != cmd_id)
//    {
//        QThread::msleep( 2000 );
//        qDebug() << "wait cmd ack " << traj_out_ack.read<uint8_t>();
//    }

//    traj_cmd_valid.write(0x0);
//    traj_cmd_id.write(cmd_id++);
//    traj_cmd_type.write(CMD_TYPE_CFG_ANGLE);
//    traj_cmd_cfg_speed.write((float)0.0008);
//    traj_cmd_cfg_acc.write((float)0.0000002);
//    traj_cmd_valid.write(0x1);

//    while (traj_out_ack.read<uint8_t>() != cmd_id)
//    {
//        QThread::msleep( 10 );
//        qDebug() << "wait cmd ack";
//    }

//    traj_cmd_valid.write(0x0);
//    traj_cmd_id.write(cmd_id++);
//    traj_cmd_type.write(CMD_TYPE_TRAJ);
//    traj_cmd_order_type.write(TRAJ_ENABLE);
//    traj_cmd_valid.write(0x1);

//    while (traj_out_ack.read<uint8_t>() != cmd_id)
//    {
//        QThread::msleep( 10 );
//        qDebug() << "wait cmd ack";
//    }

    trajectoryManager.moveToXYAbs( 0.0, 200.0, 200.0 );

    while( 1 )
    {
        QThread::msleep( 250 );
        QCoreApplication::processEvents();
    }

    return app.exec();
}
