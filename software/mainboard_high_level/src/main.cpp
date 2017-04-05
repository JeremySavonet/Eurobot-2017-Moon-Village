// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define SIMU

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QThread>

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
        qDebug()
            << "Unable to start the server:"
            << configurationServer.errorString();
        return -1;
    }

    QString ipAddress;
    QList< QHostAddress > ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for( int i = 0; i < ipAddressesList.size(); ++i )
    {
        if( ipAddressesList.at( i ) != QHostAddress::LocalHost &&
                ipAddressesList.at( i ).toIPv4Address() )
        {
            ipAddress = ipAddressesList.at( i ).toString();
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if( ipAddress.isEmpty() )
        ipAddress = QHostAddress( QHostAddress::LocalHost).toString();
    {
        qDebug()
            << "The server is running on\n\nIP:" << ipAddress << "\nport:"
            << configurationServer.serverPort();
    }

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

#ifdef SIMU
    ItemRegister configStateRegister(
        layer1,
        1 * 4,
        32 );

    configStateRegister.write( 0xffffffff );
#endif

    // INPUT REGISTER
    ItemRegister::Ptr inputRegister1 = std::make_shared< ItemRegister >( layer1, 7 * 4, 8 );
    ItemRegister::Ptr inputRegister2 = std::make_shared< ItemRegister >( layer1, 7 * 4 + 1, 8 );
    ItemRegister::Ptr inputRegister3 = std::make_shared< ItemRegister >( layer1, 7 * 4 + 2, 8 );

    // OUTOUT REGISTER OVERRIDE
    ItemRegister outputOverride( layer1, 10 * 4, 32 );
    outputOverride.write( 0x01010101 );

    ItemRegister::Ptr outputLedRegister = std::make_shared< ItemRegister >( layer1, 9 * 4, 8 );
    ItemRegister::Ptr outputIO1Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 1, 8 );
    ItemRegister::Ptr outputIO2Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 2, 8 );
    ItemRegister::Ptr outputIO3Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 3, 8 );

    // Buttons
    Input tirette( inputRegister1, "Tirette" );
    Input color( inputRegister2, "Color" );
    Input arretUrgence ( inputRegister3, "AU" );

    // Leds
    Output led( outputLedRegister, "Led" );
    Output io1( outputIO1Register, "IO1" );
    Output io2( outputIO2Register, "IO2" );
    Output io3( outputIO3Register, "IO3" );

    while( 1 )
    {
        qDebug() << "Tirette" << tirette.digitalRead();
        qDebug() << "Color" << color.digitalRead();
        qDebug() << "AU" << arretUrgence.digitalRead();

        led.digitalWrite( DigitalValue::ON );
        io1.digitalWrite( DigitalValue::ON );
        io2.digitalWrite( DigitalValue::OFF );
        io3.digitalWrite( DigitalValue::OFF );
        QThread::msleep( 250 );
        led.digitalWrite( DigitalValue::OFF );
        io1.digitalWrite( DigitalValue::OFF );
        io2.digitalWrite( DigitalValue::ON );
        io3.digitalWrite( DigitalValue::ON );
        QThread::msleep( 250 );

        QCoreApplication::processEvents();
    }

    return app.exec();
}
