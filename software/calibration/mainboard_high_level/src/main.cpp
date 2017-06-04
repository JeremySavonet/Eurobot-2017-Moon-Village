// Copyright (c) 2016-2017 All Rights Reserved WestBot

//#define SIMU

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QThread>
#include <QDateTime>

#include <hps_arm.h> // For our base address

#include <Defines.hpp>

#include <WestBot/Input.hpp>
#include <WestBot/Output.hpp>
#include <WestBot/Configuration.hpp>
#include <WestBot/ConfigurationTcpServer.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

namespace
{
    WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );

    const int DEFAULT_PORT = 57497;
}

using namespace WestBot;





#include <QTcpServer>
#include <QTcpSocket>

#include <QSerialPort>
#include <signal.h>

#include <Rtos.h>

#include "../Common/Message.h"

#define DEBUG_LEVEL 5
#define DEBUG_NAME	"SYS"
//#include <Common/DebugLevel.h>

#include <WestBot/main.hpp>


ISystem::ISystem( )
{
    {//Tcp
        server = new QTcpServer();
        connect(server, SIGNAL(newConnection()), this, SLOT(tcpNewConnection()));
        if(!server->listen(QHostAddress::Any, 5900)) {
            qDebug() <<"Server could not start!";
            //applicationExit();
        }
        connect(&tcpTimer,SIGNAL(timeout()),this,SLOT(tcpTask()));
        tcpTimer.setInterval(10);
        static u8 txBuffer[SYS_FIFO_SIZE];
        static u8 rxBuffer[SYS_FIFO_SIZE];
        fifoInit(&tcpTx,txBuffer,sizeof(txBuffer));
        fifoInit(&tcpRx,rxBuffer,sizeof(rxBuffer));
        protocolAddSendFifo( ADDR_DEST_MATLAB, MASK_DEST, &tcpTx );
        protocolAddRecvFifo( &tcpRx);
    }

    {//Gestion système et messages
		protocolAddRecvMsg( ADDR_ID_CMD_POSITION, MASK_ID, (Func2)messageCmdPosition, this );
    }

	for(u32 i=0; i<uartNb; i++)
		uart[i] = new SerialToTcp( uartName[i], uartBrg[i], uartPort[i]);

	rtosSuccess("ISystem\r\n");


}//ISystem


ISystem::~ISystem()
{
	for(u32 i=0; i<uartNb; i++)
		uart[i]->~SerialToTcp();
	tcpDisconnect();
	server->~QTcpServer();
}//~ISystem


MsgCmdPosition msg_pos;
uint8_t msg_id = 0;
int ISystem::messageCmdPosition(ISystem* sys, MsgCmdPosition *msg)
{
	qDebug() << "new cmd " << (int)msg->x << (int)msg->y << (int)(msg->teta*10);
    msg_pos.x = msg->x;
    msg_pos.y = msg->y;
	msg_pos.teta = msg->teta;

	msg_id ++ ;

    return ISUCCESS;
}//messageCmdPosition


///////////////////////////////////////////////////////////////////////////////////////////////////
/// Serial To TCP
///////////////////////////////////////////////////////////////////////////////////////////////////


SerialToTcp::SerialToTcp(const char* uart, u32 baudrate, u32 tcpPort)
{
	server = new QTcpServer();
	connect(server, SIGNAL(newConnection()), this, SLOT(tcpNewConnection()));
	if(!server->listen(QHostAddress::Any, tcpPort)) {
		qDebug() <<"Server could not start!";
	}
	connect(&timer,SIGNAL(timeout()),this,SLOT(task()));
	timer.setInterval(10);


	this->uart = new QSerialPort();
	this->uart->setPortName(uart);

	if(!this->uart->setBaudRate(baudrate)
			|| !this->uart->setDataBits(QSerialPort::Data8)
			|| !this->uart->setParity(QSerialPort::NoParity)
			|| !this->uart->setStopBits(QSerialPort::OneStop)
			|| !this->uart->setFlowControl(QSerialPort::NoFlowControl) )
	{
		rtosError("UART\tConfig (%s)\r\n", uart);
	}

	if(!this->uart->open(QIODevice::ReadWrite) )
	{
		rtosError("UART\tOpen (%s)\r\n", uart);
	}
	rtosSuccess("UART\tOpen (%s)\r\n", uart);
}//SerialToTcp


SerialToTcp::~SerialToTcp()
{
	uart->close();
	uart->~QSerialPort();
	tcpDisconnect();
}//~SerialToTcp

void SerialToTcp::tcpNewConnection()
{
	if( tcpStatus!=TCP_UNCONNECTED) {
		qDebug() << "Already connected, switching";
		tcpDisconnect();
	}

	socket = server->nextPendingConnection();
	tcpStatus = TCP_IDLE;
	timer.start();
	qDebug() << "tcp connected";
}//tcpNewConnection

void SerialToTcp::tcpDisconnect( )
{
	qDebug() <<"tcp disconnect";
	if(socket==NULL) {
		//error("tcp socket null");
	}
	else {
		qDebug() <<"tcp kill socket";
		socket->~QTcpSocket();
		socket = NULL;
	}
	qDebug() <<"tcp stop task";
	timer.stop();
	tcpStatus = TCP_UNCONNECTED;
}//tcpDisconnect

void SerialToTcp::task( )
{
	if(socket==NULL) {
		tcpDisconnect();
		return;
	}
	if(socket->state() != QAbstractSocket::ConnectedState) {
		tcpDisconnect();
		return;
	}
	int size = socket->bytesAvailable();
	if(size>0) {
		char buffer[size];
		socket->read(buffer,size);
		uart->write(buffer,size);
	}
	size = uart->bytesAvailable();
	if(size>0) {
		char buffer[size];
		uart->read(buffer,size);
		socket->write(buffer,size);
	}
}//task



///////////////////////////////////////////////////////////////////////////////////////////////////
/// TCP
///////////////////////////////////////////////////////////////////////////////////////////////////

void ISystem::tcpNewConnection()
{
    if( tcpStatus!=TCP_UNCONNECTED) {
        qDebug() << "Already connected, switching";
        if(socket!=NULL) {
            tcpTimer.stop();
            socket->close();
            socket->waitForDisconnected();
            socket->~QTcpSocket();
            socket = NULL;
        }
    }

    socket = server->nextPendingConnection();
    fifoReset(&tcpTx);
    fifoReset(&tcpRx);
    tcpStatus = TCP_IDLE;
    tcpTimer.start();
    qDebug() << "tcp connected";
}//tcpNewConnection


void ISystem::tcpDisconnect( )
{
    qDebug() <<"tcp disconnect";
    if(socket==NULL) {
        //error("tcp socket null");
    }
    else {
        qDebug() <<"tcp kill socket";
        socket->~QTcpSocket();
        socket = NULL;
    }
    qDebug() <<"tcp stop task";
    tcpTimer.stop();
    tcpStatus = TCP_UNCONNECTED;
}//tcpDisconnect


void ISystem::tcpTask( )
{
    if(socket==NULL) {
        tcpDisconnect();
        return;
    }
    if(socket->state() != QAbstractSocket::ConnectedState) {
        tcpDisconnect();
        return;
    }
    int size = socket->bytesAvailable();
    if(size>0) {
        char buffer[size];
        socket->read(buffer,size);
        fifoEnqueue(&tcpRx,buffer,size);
    }
    size = getFifoAvailable(&tcpTx);
    if(size>0) {
        char buffer[size];
        fifoDequeue(&tcpTx,buffer,size);
        socket->write(buffer,size);
    }
}//tcpTask


///////////////////////////////////////////////////////////////////////////////////////////////////
/// MAIN
///////////////////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Init the Hardware Abstraction Layer
    Hal hal;
    hal.init(); // Open the FPGA memory and load our module

    MemoryManager memoryManager;

    // Init the System manager
    SystemManager system( hal );
    system.init(); // Create the state machine

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

    // Create our layer mapping
    Memory layer1( memoryManager, PIO_N_LAYER1_BASE );
    Memory layer2( memoryManager, PIO_N_LAYER2_BASE );
    Memory layer3( memoryManager, PIO_N_LAYER3_BASE );

    // Read layer 1 registers
    qDebug() << "Layer 1 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer1.read( i * 4, 32 ), 16 );
    }

    // Read layer 2 registers
    qDebug() << "Layer 2 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer2.read( i * 4, 32 ), 16 );
    }

    // Read layer 3 registers
    qDebug() << "Layer 3 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer3.read( i * 4, 32 ), 16 );
    }

    rtosInit();
    protocolInit();
    ISystem sys;
	ignoreArg(sys);
    rtosStart();


#ifdef SIMU

    ItemRegister configStateRegister(
        layer1,
        1 * 4,
        32 );

    configStateRegister.write( 0xffffffff );
#endif

    // INPUT REGISTER
    ItemRegister inputRegister1( layer1, 7 * 4, 8 );
    ItemRegister inputRegister2( layer1, 7 * 4 + 1, 8 );
    ItemRegister inputRegister3( layer1, 7 * 4 + 2, 8 );
    ItemRegister outputRegister1( layer1, 9 * 4, 8 );
    ItemRegister outputOverrideRegister1( layer1, 10 * 4, 8 );


    outputOverrideRegister1.write(1);


#ifdef TEST_SERVO
#define SERVO_0_ARM_R_CLOSED (31217)
#define SERVO_0_ARM_R_OPEN (15590)
#define SERVO_0_ARM_R_OPEN90 (20000)
#define SERVO_0_ARM_R_DROP (30275)

#define SERVO_6_ARM_L_CLOSED (12713)
#define SERVO_6_ARM_L_OPEN (31217)
#define SERVO_6_ARM_L_OPEN90 (23000)
#define SERVO_6_ARM_L_DROP (14155)
#define SERVO_7_EJECTOR_STANDBY (31217)
#define SERVO_7_EJECTOR_EJECT (11533)

    ItemRegister servo0_en( layer1, 30 * 4+2, 8 );
    ItemRegister servo0_override( layer1, 30 * 4+3, 8 );
    ItemRegister servo0_value( layer1, 30 * 4, 16 );
    servo0_value.write(SERVO_0_ARM_R_OPEN);
    servo0_override.write(0x01);
    servo0_en.write(0x03);

    ItemRegister servo6_en( layer1, 36 * 4+2, 8 );
    ItemRegister servo6_override( layer1, 36 * 4+3, 8 );
    ItemRegister servo6_value( layer1, 36 * 4, 16 );
    servo6_value.write(SERVO_6_ARM_L_OPEN);
    servo6_override.write(0x01);
    servo6_en.write(0x3);

    ItemRegister servo7_en( layer1, 37 * 4+2, 8 );
    ItemRegister servo7_override( layer1, 37 * 4+3, 8 );
    ItemRegister servo7_value( layer1, 37 * 4, 16 );
    servo7_value.write(SERVO_7_EJECTOR_STANDBY);
    servo7_override.write(0x01);
    servo7_en.write(0x03);

//#endif


    ItemRegister esc0_en( layer1, 46 * 4+2, 8 );
    ItemRegister esc0_override( layer1, 46 * 4+3, 8 );
    ItemRegister esc0_value( layer1, 46 * 4, 16 );
    esc0_value.write(0);
    esc0_override.write(0x01);
    esc0_en.write(0x1);


    //char c = getchar();
    //if (c == 'p')
    //    qDebug() << "servo init";

    uint16_t ejector = 31717;
 //   servo6_value.write(ejector);
    QThread::msleep( 1000 );
    uint16_t right = 31717;
    uint16_t left = 12713;

 //   servo7_value.write(left);
 //   servo0_value.write(right);

    uint8_t value_servo = 0;



    QThread::msleep( 500 );

    int16_t esc_value=0;


    while(1) {

		if (esc_value >= -130)
			esc_value-=1;

			//esc_value%=180;

        esc0_value.write(esc_value);

        //esc_value;
        outputRegister1.write(inputRegister2.read<uint8_t>());
        QThread::msleep( 90 );
        qDebug() << "coucou " << esc_value << " " << inputRegister1.read<uint8_t>() << " " << inputRegister2.read<uint8_t>() << " " << inputRegister3.read<uint8_t>() << " ";

    }

//#if 0
    while(1) {

        servo0_value.write(SERVO_0_ARM_R_CLOSED);
        servo6_value.write(SERVO_6_ARM_L_CLOSED);

        QThread::msleep( 1000 );

        servo0_value.write(SERVO_0_ARM_R_DROP);
        servo6_value.write(SERVO_6_ARM_L_DROP);

        QThread::msleep( 1000 );

        servo7_value.write(SERVO_7_EJECTOR_EJECT);

        QThread::msleep( 1000 );

        servo0_value.write(SERVO_0_ARM_R_OPEN90);
        servo6_value.write(SERVO_6_ARM_L_OPEN90);

        QThread::msleep( 1000 );

        servo7_value.write(SERVO_7_EJECTOR_STANDBY);

        QThread::msleep( 5000 );


        qDebug() << "servo change";

    }
#endif

    ItemRegister motor_0_override( layer1, 12 * 4+3, 8 );
    ItemRegister motor_0_value( layer1, 12 * 4, 16 );
    motor_0_override.write(0);
    motor_0_value.write(0);

    ItemRegister motor_1_override( layer1, 13 * 4+3, 8 );
    ItemRegister motor_1_value( layer1, 13 * 4, 16 );
    motor_1_override.write(0);
    motor_1_value.write(0);

    ItemRegister motor_5_override( layer1, 17 * 4+3, 8 );
    ItemRegister motor_5_value( layer1, 17 * 4, 16 );
    motor_5_override.write(1);
	motor_5_value.write(25000/2);

    ItemRegister color_red( layer1, 51 * 4, 16 );

    ItemRegister distance_0( layer1, 53 * 4, 32 );

    ItemRegister adc0( layer1, 38 * 4, 32 );
    ItemRegister adc1( layer1, 39 * 4, 32 );


	/*while (1) {
        QThread::msleep( 500 );
        qDebug() << "coucou " << color_red.read<uint16_t>() << distance_0.read<uint32_t>() << adc0.read<uint32_t>() << adc1.read<uint32_t>();
	}*/



    ItemRegister motor_2_override( layer1, 14 * 4+3, 8 );
    ItemRegister motor_2_value( layer1, 14 * 4, 16 );
    motor_2_value.write(0);
    motor_2_override.write(0x00);

    ItemRegister motor_3_override( layer1, 15 * 4+3, 8 );
    ItemRegister motor_3_inv( layer1, 15 * 4+2, 8 );
    ItemRegister motor_3_value( layer1, 15 * 4, 16 );
    motor_3_value.write(0);
    motor_3_inv.write(1);
    motor_3_override.write(0x00);



    ItemRegister qei_0_value( layer1, 24 * 4, 16 );
    ItemRegister qei_0_ref( layer1, 24 * 4+2, 8 );


    ItemRegister qei_4_value( layer1, 28 * 4, 16 );
    ItemRegister qei_4_ref( layer1, 28 * 4+2, 8 );


    ItemRegister pid_carrousel_en( layer2, 38 * 4, 8 );
    ItemRegister pid_carrousel_override( layer2, 38 * 4+1, 8 );

    ItemRegister pid_carrousel_p( layer2, 39 * 4, 32 );
    ItemRegister pid_carrousel_i( layer2, 40 * 4, 32 );
    ItemRegister pid_carrousel_d( layer2, 41 * 4, 32 );
    ItemRegister pid_carrousel_speed( layer2,42 * 4, 32 );
    ItemRegister pid_carrousel_acc( layer2, 43 * 4, 32 );
    ItemRegister pid_carrousel_sat( layer2, 44 * 4, 32 );

    ItemRegister pid_carrousel_pos( layer2, 45 * 4, 32 );
    ItemRegister pid_carrousel_target( layer2, 46 * 4, 32 );

    ItemRegister carroussel_ref( layer2, 48 * 4, 32 );

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
	pid_distance_p.write((float)(8000.0));
	pid_distance_i.write((float)10.0);
    pid_distance_d.write((float)0.0);
    pid_distance_speed.write((float)0.01);
    pid_distance_acc.write((float)0.0001);
    pid_distance_sat.write(25000);

    pid_distance_target.write(pid_distance_pos.read<float>());
    pid_distance_en.write(1);
    pid_distance_target.write(pid_distance_pos.read<float>());



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
	pid_angle_p.write((float)(500000.0*0.75));

	pid_angle_i.write((float)(50.0));
    pid_angle_d.write((float)0.0);
    pid_angle_speed.write((float)0.0001);
    pid_angle_acc.write((float)0.00000002);
    pid_angle_sat.write(25000);

    pid_angle_target.write(pid_angle_pos.read<float>());
    pid_angle_en.write(1);


    ItemRegister odo_x( layer2, 14 * 4, 16 );
    ItemRegister odo_y( layer2, 14 * 4+2, 16 );
    ItemRegister odo_teta( layer2, 13 * 4+2, 16);


#define CMD_TYPE_TRAJ 0x0
#define CMD_TYPE_CFG_DISTANCE 0x1
#define CMD_TYPE_CFG_ANGLE 0x2
#define CMD_TYPE_CFG_WND 0x3


#define TRAJ_DISABLE 0
#define TRAJ_ENABLE 1
#define TRAJ_STOP 2
#define TRAJ_HARDSTOP 3
#define TRAJ_D_REL 4
#define TRAJ_ONLY_D_REL 5
#define TRAJ_A_REL 6
#define TRAJ_A_ABS 7
#define TRAJ_ONLY_A_REL 8
#define TRAJ_ONLY_A_ABS 9
#define TRAJ_D_A_REL 10
#define TRAJ_TURNTO_XY 11
#define TRAJ_TURNTO_XY_BEHIND 12
#define TRAJ_GOTO_XY_ABS 13
#define TRAJ_GOTO_FORWARD_XY_ABS 14
#define TRAJ_GOTO_BACKWARD_XY_ABS 15
#define TRAJ_GOTO_D_A_REL 16
#define TRAJ_GOTO_XY_REL 17

    ItemRegister traj_freq_hz( layer3, 2 * 4, 16 );
    traj_freq_hz.write(10);

    ItemRegister traj_cmd_valid( layer3, 11 * 4, 8 );
    ItemRegister traj_cmd_id( layer3, 11 * 4+1, 8 );
    ItemRegister traj_cmd_type( layer3, 11 * 4+2, 8 );
    ItemRegister traj_cmd_order_type( layer3, 11 * 4+3, 8 );


    ItemRegister traj_cmd_wnd_distance( layer3, 12 * 4, 32 );
    ItemRegister traj_cmd_wnd_angle_deg( layer3, 13 * 4, 32 );
    ItemRegister traj_cmd_wnd_angle_start_deg( layer3, 14 * 4, 32 );


    ItemRegister traj_cmd_cfg_speed( layer3, 12 * 4, 32 );
    ItemRegister traj_cmd_cfg_acc( layer3, 13 * 4, 32 );

    ItemRegister traj_cmd_pos_teta( layer3, 12 * 4, 32 );
    ItemRegister traj_cmd_pos_x( layer3, 13 * 4, 32 );
    ItemRegister traj_cmd_pos_y( layer3, 14 * 4, 32 );

    ItemRegister traj_cmd_d_distance( layer3, 12 * 4, 32 );

    ItemRegister traj_out_ack( layer3, 15 * 4, 8 );
    ItemRegister traj_out_state( layer3, 15 * 4+1, 8 );
    ItemRegister traj_out_blocked( layer3, 15 * 4+2, 8 );
    ItemRegister traj_out_in_window( layer3, 15 * 4+3, 8 );


    /** State of the trajectory manager. */
    enum trajectory_state {
        READY, /**< Ready, waiting for a trajectory. */

        /* simple trajectories */
        RUNNING_A, /**< Turning without moving. */
        RUNNING_D, /**< Going straight. */
        RUNNING_AD, /**< Going forward and turning at the same time. */

        /* trajectories using events */
        RUNNING_XY_START,        /**< A trajectory order was given, preparing to go. */
        RUNNING_XY_ANGLE,        /**< Doing a preliminary turn before starting to move. */
        RUNNING_XY_ANGLE_OK,     /**< Angle is now ok, move forward. */
        RUNNING_XY_F_START,      /**< Same as RUNNING_XY_START but forward only. */
        RUNNING_XY_F_ANGLE,      /**< Same as RUNNING_XY_ANGLE but forward only. */
        RUNNING_XY_F_ANGLE_OK,   /**< Same as RUNNING_XY_ANGLE_OK but forward only. */
        RUNNING_XY_B_START,      /**< Same as RUNNING_XY_START but backward only. */
        RUNNING_XY_B_ANGLE,      /**< Same as RUNNING_XY_ANGLE but backward only. */
        RUNNING_XY_B_ANGLE_OK,   /**< Same as RUNNING_XY_B_ANGLE_OK but backward only. */

        /* circle */
        RUNNING_CIRCLE,          /**< Running a circle trajectory. */

        /* line */
        RUNNING_LINE,            /**< Following a line. */

        /* clitoid */
        RUNNING_CLITOID_LINE,    /**< Running a clitoid (line->circle->line) in the line part. */
        RUNNING_CLITOID_CURVE,   /**< Running a clitoid in the curve part. */
    };
    enum trajectory_state state;
    uint8_t in_window;
    uint8_t cmd_id = traj_out_ack.read<uint8_t>();



    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_DISABLE);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }

    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_CFG_WND);
	traj_cmd_wnd_distance.write((float)200.0);
	traj_cmd_wnd_angle_deg.write((float)2.0);
	traj_cmd_wnd_angle_start_deg.write((float)70.0);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 2000 );
        qDebug() << "wait cmd ack " << traj_out_ack.read<uint8_t>();
    }

    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_CFG_DISTANCE);
	traj_cmd_cfg_speed.write((float)(0.14*1.5 ));
	traj_cmd_cfg_acc.write((float)(0.00001*4.0 ));
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 2000 );
        qDebug() << "wait cmd ack " << traj_out_ack.read<uint8_t>();
    }

    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_CFG_ANGLE);
    traj_cmd_cfg_speed.write((float)0.0008);
	traj_cmd_cfg_acc.write((float)(0.0000002*0.75));
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }



    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_ENABLE);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }

#if 0

    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_A_REL);
    traj_cmd_pos_teta.write((float)90.0);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0);


    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_GOTO_XY_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_pos_x.write((float)1200.0);
    traj_cmd_pos_y.write((float)200.0);

    traj_cmd_valid.write(0x1);


    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0);


    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_GOTO_XY_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_pos_x.write((float)500.0);
    traj_cmd_pos_y.write((float)200.0);

    traj_cmd_valid.write(0x1);


    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0);



    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_GOTO_XY_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_pos_x.write((float)0.0);
    traj_cmd_pos_y.write((float)0.0);

    traj_cmd_valid.write(0x1);


    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0);


    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_A_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0);



    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_DISABLE);
    traj_cmd_valid.write(0x1);

    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }

#endif

#if 0

    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_GOTO_XY_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_pos_x.write((float)500.0);
    traj_cmd_pos_y.write((float)0.0);

    traj_cmd_valid.write(0x1);


    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0 && state != READY);



    traj_cmd_valid.write(0x0);
    traj_cmd_id.write(cmd_id++);
    traj_cmd_type.write(CMD_TYPE_TRAJ);
    traj_cmd_order_type.write(TRAJ_GOTO_XY_ABS);
    traj_cmd_pos_teta.write((float)0.0);
    traj_cmd_pos_x.write((float)0.0);
    traj_cmd_pos_y.write((float)0.0);

    traj_cmd_valid.write(0x1);



    while (traj_out_ack.read<uint8_t>() != cmd_id)
    {
        QThread::msleep( 10 );
        qDebug() << "wait cmd ack";
    }
    do {
        QThread::msleep( 100 );
        state = (trajectory_state)traj_out_state.read<uint8_t>();
        in_window = traj_out_in_window.read<uint8_t>();
        qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
    } while (in_window == 0 && state != READY);


#endif


    uint8_t current_id = 0;

    while( 1 )
    {
        if (current_id != msg_id)
        {
            current_id = msg_id;

            traj_cmd_valid.write(0x0);
            traj_cmd_id.write(cmd_id++);
            traj_cmd_type.write(CMD_TYPE_TRAJ);

			if (msg_pos.teta == 0.0) {
				qDebug() << "go xy";
				traj_cmd_order_type.write(TRAJ_GOTO_FORWARD_XY_ABS);
				traj_cmd_pos_teta.write((float)0.0);
				traj_cmd_pos_x.write((float)msg_pos.x);
				traj_cmd_pos_y.write((float)msg_pos.y);
			} else {
				qDebug() << "go a abs";
				traj_cmd_order_type.write(TRAJ_A_ABS);
				traj_cmd_pos_teta.write((float)msg_pos.teta);
			}


            traj_cmd_valid.write(0x1);


            while (traj_out_ack.read<uint8_t>() != cmd_id)
            {
                QThread::msleep( 10 );
                qDebug() << "wait cmd ack";
            }

            do {
				QThread::msleep( 100 );
				QCoreApplication::processEvents();

                MsgStatus msg;
                msg.x = (float)odo_x.read<int16_t>();
                msg.y = (float)odo_y.read<int16_t>();
                msg.progress = 0;
				//sendMsg(ADDR_DEST_MATLAB|ADDR_ID_STATUS,&msg,sizeof(msg));

                state = (trajectory_state)traj_out_state.read<uint8_t>();
                in_window = traj_out_in_window.read<uint8_t>();
                qDebug() << "wait traj ready " << state << " in windows: " << in_window << "x/y/teta:" << odo_x.read<int16_t>() << "/" << odo_y.read<int16_t>() << "/" << odo_teta.read<int16_t>();
            } while (in_window == 0 && state != READY);


            MsgStatus msg;
            msg.x = (float)odo_x.read<int16_t>();
            msg.y = (float)odo_y.read<int16_t>();
            msg.progress = 100;
            sendMsg(ADDR_DEST_MATLAB|ADDR_ID_STATUS,&msg,sizeof(msg));
        }




        QCoreApplication::processEvents();
    }

    return app.exec();
}
