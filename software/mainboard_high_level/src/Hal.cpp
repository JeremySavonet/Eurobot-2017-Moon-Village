// Copyright (c) 2016-2017 All Rights Reserved WestBot

#define DEBUG

#include <QDebug>

#include <hps_arm.h> // For our base address

#include <WestBot/Hal.hpp>

using namespace WestBot;

Hal::Hal()
    : _layer1( _memoryManager , PIO_N_LAYER1_BASE )
    , _layer2( _memoryManager, PIO_N_LAYER2_BASE )
    , _layer3( _memoryManager, PIO_N_LAYER3_BASE )
    // Layer 1
    , _resetL1( _layer1, 0, 32  )
    , _initOkL1( _layer1, 1 * 4, 8 )
    , _modeSimu( _layer1, 1 * 4 + 1, 8 )
    , _voltage24V( _layer1, 2 * 4, 32 )
    , _voltageA12V( _layer1, 3 * 4, 32 )
    , _voltageA5V( _layer1, 4 * 4, 32 )
    , _voltage12V( _layer1, 5 * 4, 32 )
    , _buzzer( _layer1, 6 * 4, 8 )
    , _buzzerOverride( _layer1, 6 * 4 +1, 8 )
    , _input0( _layer1, 7 * 4, 8 )
    , _input1( _layer1, 7 * 4 + 1, 8 )
    , _input2( _layer1, 7 * 4  + 2, 8  )
    , _input3( _layer1, 7 * 4 + 3, 8 )
    , _inputOverride( _layer1, 8 * 4, 32 )
    , _output0( _layer1, 9 * 4, 8 )
    , _output1( _layer1, 9 * 4 + 1, 8 )
    , _output2( _layer1, 9 * 4 + 2, 8 )
    , _output3( _layer1, 9 * 4 + 3, 8 )
    , _outputOverride( _layer1, 10 * 4, 32 )
    , _s0( _layer1, 30 * 4, 16 )
    , _s0Enable( _layer1, 30 * 4 + 2, 8 )
    , _s0Override( _layer1, 30 * 4 + 3, 8 )
    , _s1( _layer1, 31 * 4, 16 )
    , _s1Enable( _layer1, 31 * 4 + 2, 8 )
    , _s1Override( _layer1, 31 * 4 + 3, 8 )
    , _s2( _layer1, 32 * 4, 16 )
    , _s2Enable( _layer1, 32 * 4 + 2, 8 )
    , _s2Override( _layer1, 32 * 4 + 3, 8 )
    , _s3( _layer1, 33 * 4, 16 )
    , _s3Enable( _layer1, 33 * 4 + 2, 8)
    , _s3Override( _layer1, 33 * 4 + 3, 8 )
    , _s4( _layer1, 34 * 4, 16 )
    , _s4Enable( _layer1, 34 * 4 + 2, 8 )
    , _s4Override( _layer1, 34 * 4 + 3, 8 )
    , _s5( _layer1, 35 * 4, 16 )
    , _s5Enable( _layer1, 35 * 4 + 2, 8 )
    , _s5Override( _layer1, 35 * 4 + 3, 8 )
    , _s6( _layer1, 36 * 4, 16 )
    , _s6Enable( _layer1, 36 * 4 + 2, 8 )
    , _s6Override( _layer1, 36 * 4 + 3, 8 )
    , _s7( _layer1, 37 * 4, 16 )
    , _s7Enable( _layer1, 37 * 4 + 2, 8 )
    , _s7Override( _layer1, 37 * 4 + 3, 8 )

    // Layer 2
    , _odometryTheta( _layer2, 13 * 4 + 2, 16 )
    , _odometryX( _layer2, 14 * 4, 16 )
    , _odometryY( _layer2, 14 * 4 + 2, 16 )
    , _pidDistanceInverted( _layer2, 16 * 4 + 2, 8 )
    , _pidDistanceKp( _layer2, 17 * 4, 32 )
    , _pidDistanceKi( _layer2, 18 * 4, 32 )
    , _pidDistanceKd( _layer2, 19 * 4, 32 )
    , _pidAngleInverted( _layer2, 27 * 4 + 2, 8 )
    , _pidAngleKp( _layer2, 28 * 4, 32 )
    , _pidAngleKi( _layer2, 29 * 4, 32 )
    , _pidAngleKd( _layer2, 30 * 4, 32 )
    , _carrouselPidPeriod( _layer2, 37 * 4, 32 )
    , _carrouselEnable( _layer2, 38 * 4, 8 )
    , _carrouselOverride(  _layer2, 38 * 4 + 1, 8 )
    , _carrouselPidKp( _layer2, 39 * 4, 32 )
    , _carrouselPidKi( _layer2, 40 * 4, 32 )
    , _carrouselPidKd(  _layer2, 41 * 4, 32 )
    , _carrouselSpeed(  _layer2, 42 * 4, 32 )
    , _carrouselAcc(  _layer2, 43 * 4, 32 )
    , _carrouselOutputSaturation(  _layer2, 44 * 4, 32 )
    , _carrouselPosition(  _layer2, 45 * 4, 32 )
    , _carrouselTarget( _layer2, 46 * 4, 32 )
    , _carrouselLastReference( _layer2, 48 * 4, 32 )
{
}

Hal::~Hal()
{
    _layer1.reset();
    _layer2.reset();
    _layer3.reset();
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

    qInfo() << "Successfully initialized Hal module";
}
