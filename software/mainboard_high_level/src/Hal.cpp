// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define DEBUG
#define SIMU

#include <QDebug>

#include <hps_arm.h> // For our base address

#include <WestBot/Hal.hpp>

#define ADD_REGISTER( name, layerRegister ) \
    {                                       \
        name, layerRegister                 \
    }

using namespace WestBot;

Hal::Hal()
    : _layer1( _memoryManager , PIO_N_LAYER1_BASE )
    , _layer2( _memoryManager, PIO_N_LAYER2_BASE )
    , _layer3( _memoryManager, PIO_N_LAYER3_BASE )
{
}

Hal::~Hal()
{
}

void Hal::init()
{
#ifdef DEBUG
    // Read layer 1 registers
    qDebug() << "Layer 1 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( _layer1.read( i * 4, 32 ), 16 );
    }

    // Read layer 2 registers
    qDebug() << "Layer 2 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( _layer2.read( i * 4, 32 ), 16 );
    }

    // Read layer 3 registers
    qDebug() << "Layer 3 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( _layer3.read( i * 4, 32 ), 16 );
    }
#endif

#ifdef SIMU
    ItemRegister configStateRegister( _layer1, 1 * 4, 32 );
    configStateRegister.write( 0xffffffff );
#endif

    //// INPUT REGISTER
    //ItemRegister::Ptr inputRegister1 = std::make_shared< ItemRegister >( layer1, 7 * 4, 8 );
    //ItemRegister::Ptr inputRegister2 = std::make_shared< ItemRegister >( layer1, 7 * 4 + 1, 8 );
    //ItemRegister::Ptr inputRegister3 = std::make_shared< ItemRegister >( layer1, 7 * 4 + 2, 8 );

    //// OUTOUT REGISTER OVERRIDE
    //ItemRegister outputOverride( layer1, 10 * 4, 32 );
    //outputOverride.write( 0x01010101 );

    //ItemRegister::Ptr outputLedRegister = std::make_shared< ItemRegister >( layer1, 9 * 4, 8 );
    //ItemRegister::Ptr outputIO1Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 1, 8 );
    //ItemRegister::Ptr outputIO2Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 2, 8 );
    //ItemRegister::Ptr outputIO3Register = std::make_shared< ItemRegister >( layer1, 9 * 4 + 3, 8 );

    //// Buttons
    //Input tirette( inputRegister1, "Tirette" );
    //Input color( inputRegister2, "Color" );
    //Input arretUrgence ( inputRegister3, "AU" );

    //// Leds
    //Output led( outputLedRegister, "Led" );
    //Output io1( outputIO1Register, "IO1" );
    //Output io2( outputIO2Register, "IO2" );
    //Output io3( outputIO3Register, "IO3" );

    qInfo() << "Successfully initialized Hal module";
}

ItemRegister::Ptr Hal::itemWithId( const QString& id )
{
    return _registersMap.value( id );
}

QMap< QString, ItemRegister::Ptr >& Hal::items()
{
    return _registersMap;
}
