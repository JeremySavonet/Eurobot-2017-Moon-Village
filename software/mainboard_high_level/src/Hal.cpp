// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define DEBUG
#define SIMU

#include <QDebug>

#include <hps_arm.h> // For our base address

#include <WestBot/Hal.hpp>

using namespace WestBot;

Hal::Hal()
    : _layer1( _memoryManager , PIO_N_LAYER1_BASE )
    , _layer2( _memoryManager, PIO_N_LAYER2_BASE )
    , _layer3( _memoryManager, PIO_N_LAYER3_BASE )
{
}

Hal::~Hal()
{
    _layer1.reset();
    _layer2.reset();
    _layer3.reset();

    _registersMap.clear();
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
    ItemRegister::Ptr configStateRegister = std::make_shared< ItemRegister >( _layer1, 1 * 4, 32 );
    _registersMap.insert( "CONFIG", configStateRegister );
#endif

    // INPUT REGISTER
    ItemRegister::Ptr inputRegister1 = std::make_shared< ItemRegister >( _layer1, 7 * 4, 8 );
    ItemRegister::Ptr inputRegister2 = std::make_shared< ItemRegister >( _layer1, 7 * 4 + 1, 8 );
    ItemRegister::Ptr inputRegister3 = std::make_shared< ItemRegister >( _layer1, 7 * 4 + 2, 8 );

    _registersMap.insert( "IN1", inputRegister1 );
    _registersMap.insert( "IN2", inputRegister2 );
    _registersMap.insert( "IN3", inputRegister3 );

    // OUTOUT REGISTER OVERRIDE
    ItemRegister::Ptr outputOverride =  std::make_shared< ItemRegister >( _layer1, 10 * 4, 32 );
    outputOverride->write( 0x01010101 );
    _registersMap.insert( "OUTOVERRIDE", outputOverride );

    // OUTPUT REGISTER
    ItemRegister::Ptr outputLedRegister = std::make_shared< ItemRegister >( _layer1, 9 * 4, 8 );
    ItemRegister::Ptr outputIO1Register = std::make_shared< ItemRegister >( _layer1, 9 * 4 + 1, 8 );
    ItemRegister::Ptr outputIO2Register = std::make_shared< ItemRegister >( _layer1, 9 * 4 + 2, 8 );
    ItemRegister::Ptr outputIO3Register = std::make_shared< ItemRegister >( _layer1, 9 * 4 + 3, 8 );

    _registersMap.insert( "OUT1", outputLedRegister );
    _registersMap.insert( "OUT2", outputIO1Register );
    _registersMap.insert( "OUT3", outputIO2Register );
    _registersMap.insert( "OUT4", outputIO3Register );

    ItemRegister::Ptr carrouselPeriod =
        std::make_shared< ItemRegister >( _layer2, 37 * 4, 16 );
    ItemRegister::Ptr carrouselEnable =
        std::make_shared< ItemRegister >( _layer2, 38 * 4, 8 );
    ItemRegister::Ptr carrouselState =
        std::make_shared< ItemRegister >( _layer2, 38 * 4 + 1, 8 );
    ItemRegister::Ptr carrouselKp =
        std::make_shared< ItemRegister >( _layer2, 39 * 4, 32 );
    ItemRegister::Ptr carrouselKi =
        std::make_shared< ItemRegister >( _layer2, 40 * 4, 32 );
    ItemRegister::Ptr carrouselKd =
        std::make_shared< ItemRegister >( _layer2, 41 * 4, 32 );
    ItemRegister::Ptr carrouselSpeed =
        std::make_shared< ItemRegister >( _layer2, 42 * 4, 32 );
    ItemRegister::Ptr carrouselAcc =
        std::make_shared< ItemRegister >( _layer2, 43 * 4, 32 );
    ItemRegister::Ptr carrouselSat =
        std::make_shared< ItemRegister >( _layer2, 44 * 4, 32 );
    ItemRegister::Ptr carrouselCurrentMeasure =
        std::make_shared< ItemRegister >( _layer2, 45 * 4, 32 );
    ItemRegister::Ptr carrouselCurrentTarget =
        std::make_shared< ItemRegister >( _layer2, 46 * 4, 32 );
    ItemRegister::Ptr carrouselCurrentOutputVal =
        std::make_shared< ItemRegister >( _layer2, 47 * 4, 32 );
    ItemRegister::Ptr carrouselLastRef =
        std::make_shared< ItemRegister >( _layer2, 48 * 4, 32 );

    _registersMap.insert( "CarrouselPeriod", carrouselPeriod );
    _registersMap.insert( "CarrouselEnable", carrouselEnable );
    _registersMap.insert( "CarrouselState", carrouselState );
    _registersMap.insert( "CarrouselKp", carrouselKp );
    _registersMap.insert( "CarrouselKi", carrouselKi );
    _registersMap.insert( "CarrouselKd", carrouselKd );
    _registersMap.insert( "CarrouselSpeed", carrouselSpeed );
    _registersMap.insert( "CarrouselAcc", carrouselAcc );
    _registersMap.insert( "CarrouselSat", carrouselSat );
    _registersMap.insert( "CarrouselCurrentMeasure", carrouselCurrentMeasure );
    _registersMap.insert( "CarrouselCurrentTarget", carrouselCurrentTarget );
    _registersMap.insert( "CarrouselCurrentOutputVal", carrouselCurrentOutputVal );
    _registersMap.insert( "CarrouselLastRef", carrouselLastRef );

    // TODO: Need to insert all our register in the map
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
