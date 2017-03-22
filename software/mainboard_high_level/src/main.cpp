// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define SIMU
#define DEBUG

#include <memory>

#include <QCoreApplication>
#ifdef DEBUG
#include <QDebug>
#endif
#include <QMap>
#include <QThread>

#include <hps_arm.h> // For our base address

#include <Defines.hpp>

#include <WestBot/Configuration.hpp>
#include <WestBot/GameManager.hpp>
#include <WestBot/Input.hpp>
#include <WestBot/ItemRegister.hpp>
#include <WestBot/Memory.hpp>
#include <WestBot/MemoryManager.hpp>
#include <WestBot/Output.hpp>

#define ADD_REGISTER( name, layerRegister ) \
    {                                       \
        name, layerRegister                 \
    }

namespace
{
    WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );
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

    // Manage the LW_BRIDGE
    MemoryManager memoryManager;

    // Create our layer mapping
    Memory layer1( memoryManager, PIO_N_LAYER1_BASE );
    Memory layer2( memoryManager, PIO_N_LAYER2_BASE );
    Memory layer3( memoryManager, PIO_N_LAYER3_BASE );

    static QMap< QString, ItemRegister::Ptr > layer1Registers
    {
        ADD_REGISTER(
            "Reset",
            std::make_shared< ItemRegister >( layer1, 0, 32 ) ),
        ADD_REGISTER(
            "Simu",
            std::make_shared< ItemRegister >( layer1, 1 * 4, 32 ) ),
        ADD_REGISTER(
            "Tirette",
            std::make_shared< ItemRegister >( layer1, 7 * 4, 8 ) ),
        ADD_REGISTER(
            "Color",
            std::make_shared< ItemRegister >( layer1, 7 * 4 + 1, 8 ) ),
        ADD_REGISTER(
            "AU",
            std::make_shared< ItemRegister >( layer1, 7 * 4 + 2, 8 ) ),
        ADD_REGISTER(
            "In3",
            std::make_shared< ItemRegister >( layer1, 7 * 4 + 3, 8 ) ),
        ADD_REGISTER(
            "InputOverride",
            std::make_shared< ItemRegister >( layer1, 8 * 4, 32 ) ),
        ADD_REGISTER(
            "Led",
            std::make_shared< ItemRegister >( layer1, 9 * 4, 8 ) ),
        ADD_REGISTER(
            "Out1",
            std::make_shared< ItemRegister >( layer1, 9 * 4 + 1, 8 ) ),
        ADD_REGISTER(
            "Out2",
            std::make_shared< ItemRegister >( layer1, 9 * 4 + 2, 8 ) ),
        ADD_REGISTER(
            "Out3",
            std::make_shared< ItemRegister >( layer1, 9 * 4 + 3, 8 ) ),
        ADD_REGISTER(
            "OutputOverride",
            std::make_shared< ItemRegister >( layer1, 10, 32 ) )
    };

#ifdef SIMU
    layer1Registers[ "Simu" ]->write( 0xffffffff );
#endif

    layer1Registers[ "OutputOverride" ]->write( 0x01010101 );

    // Buttons
    Input::Ptr tirette =
        std::make_shared< Input >( layer1Registers[ "Tirette" ], "Tirette" );
    Input::Ptr color =
        std::make_shared< Input >( layer1Registers[ "Color" ], "Color" );
    Input::Ptr arretUrgence =
        std::make_shared< Input >( layer1Registers[ "AU" ], "AU" );

    // Leds
    Output led( layer1Registers[ "Led" ], "Led" );
    Output io1( layer1Registers[ "Out1" ], "IO1" );
    Output io2( layer1Registers[ "Out2" ], "IO2" );
    Output io3( layer1Registers[ "Out3" ], "IO3" );

    GameManager game( tirette, color, arretUrgence );

    while(1)
    {
        qDebug() << "TIRETTE:" << tirette->digitalRead();
        qDebug() << "COLOR:" << color->digitalRead();
        qDebug() << "AU:" << arretUrgence->digitalRead();

        led.digitalWrite( DigitalValue::ON );
        io1.digitalWrite( DigitalValue::OFF );
        io2.digitalWrite( DigitalValue::ON );
        io3.digitalWrite( DigitalValue::OFF );
        QThread::msleep( 250 );
        led.digitalWrite( DigitalValue::OFF );
        io1.digitalWrite( DigitalValue::ON );
        io2.digitalWrite( DigitalValue::OFF );
        io3.digitalWrite( DigitalValue::ON );
        QThread::msleep( 250 );

        QCoreApplication::processEvents();
    }

    return app.exec();
}
