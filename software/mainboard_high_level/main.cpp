// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define SIMU
#define DEBUG

#include <memory>

#include <QCoreApplication>
#ifdef DEBUG
#include <QDebug>
#endif
#include <QThread>

#include "hps_arm.h" // For our base address

#include "GameManager.hpp"
#include "Input.hpp"
#include "ItemRegister.hpp"
#include "Memory.hpp"
#include "MemoryManager.hpp"
#include "Output.hpp"

using namespace WestBot;

QDebug operator<<( QDebug debug, Input::Value value );

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Manage the LW_BRIDGE
    MemoryManager memoryManager;

    // Create our layer mapping
    Memory layer1( memoryManager, PIO_N_LAYER1_BASE );
    Memory layer2( memoryManager, PIO_N_LAYER2_BASE );
    Memory layer3( memoryManager, PIO_N_LAYER3_BASE );

#ifdef DEBUG
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
#endif

#ifdef SIMU
    ItemRegister configStateRegister(
        layer1,
        1 * 4,
        32 );

    configStateRegister.write( 0xffffffff );
#endif

    // INPUT REGISTER
    ItemRegister inputRegister( layer1, 7 * 4, 32 );

    // OUTOUT REGISTER
    ItemRegister outputOverride( layer1, 10 * 4, 32 );
    outputOverride.write( 0x01010101 );

    ItemRegister outputLedRegister( layer1, 9 * 4, 8 );
    ItemRegister outputIO1Register( layer1, 9 * 4 + 1, 8 );
    ItemRegister outputIO2Register( layer1, 9 * 4 + 2, 8 );
    ItemRegister outputIO3Register( layer1, 9 * 4 + 3, 8 );

    // M0
    ItemRegister pwm0Override( layer1, 12 * 4 + 3, 8 );
    pwm0Override.write( 0x01 );

    ItemRegister pwm0( layer1, 12 * 4, 16 );
    pwm0.write( 32000 );

    // Buttons
    Input::Ptr tirette = std::make_shared< Input >( inputRegister, Input::InputType::Start, "Tirette" );
    Input::Ptr color = std::make_shared< Input >( inputRegister, Input::InputType::Color, "Color" );
    Input::Ptr arretUrgence = std::make_shared< Input >( inputRegister, Input::InputType::Stop, "AU" );

    // Leds
    Output led( outputLedRegister, Output::OutputType::Led, "Led" );
    Output io1( outputIO1Register, Output::OutputType::IO1, "IO1" );
    Output io2( outputIO2Register, Output::OutputType::IO2, "IO2" );
    Output io3( outputIO3Register, Output::OutputType::IO3, "IO3" );

    GameManager game( tirette, color, arretUrgence );

    while(1)
    {
        QCoreApplication::processEvents();
    }

    /*
    while( 1 )
    {
        qDebug() << "Tirette:" << tirette->digitalRead();
        qDebug() << "Color:" << color->digitalRead();
        qDebug() << "AU:" << arretUrgence->digitalRead();

        led.digitalWrite( Output::Value::ON );
        io1.digitalWrite( Output::Value::OFF );
        io2.digitalWrite( Output::Value::ON );
        io3.digitalWrite( Output::Value::OFF );
        QThread::msleep( 250 );

        led.digitalWrite( Output::Value::OFF );
        io1.digitalWrite( Output::Value::ON );
        io2.digitalWrite( Output::Value::OFF );
        io3.digitalWrite( Output::Value::ON );
        QThread::msleep( 250 );
    }
    */

    return app.exec();
}

QDebug operator<<( QDebug debug, Input::Value value )
{
    switch( value )
    {
    case Input::Value::OFF:
        debug << "OFF";
        break;

    case Input::Value::ON:
        debug << "ON";
        break;
    }
    return debug;
}
