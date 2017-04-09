// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QThread>

#include <WestBot/Carrousel.hpp>

using namespace WestBot;

namespace
{
    const int THRESHOLD = 5;
}

Carrousel::Carrousel( Hal& hal )
    : _hal( hal )
    , _periodReg( _hal.itemWithId( "CarrouselPeriod" ) )
    , _enableReg( _hal.itemWithId( "CarrouselEnable" ) )
    , _stateReg( _hal.itemWithId( "CarrouselState" ) )
    , _kpReg( _hal.itemWithId( "CarrouselKp" ) )
    , _kiReg( _hal.itemWithId( "CarrouselKi" ) )
    , _kdReg( _hal.itemWithId( "CarrouselKd" ) )
    , _speedReg( _hal.itemWithId( "CarrouselSpeed" ) )
    , _accReg( _hal.itemWithId( "CarrouselAcc" ) )
    , _outputSatReg( _hal.itemWithId( "CarrouselSat" ) )
    ,  _currentMeasureReg( _hal.itemWithId( "CarrouselCurrentMeasure" ) )
    , _currentTargetReg( _hal.itemWithId( "CarrouselCurrentTarget" ) )
    , _lastRefReg( _hal.itemWithId( "CarrouselLastRef" ) )
    , _reference( 0 )
{
}

bool Carrousel::init()
{
    // Disable the module first
    setEnable( false );

    // Set PID gains
    setGainKd( 0.0 );
    setGainKi( 0.0 );
    setGainKd( 0.0 );

    setSpeed( 0.0 );
    setAcceleration( 0.0 );

    int32_t initialRef = lastReference();
    int32_t initialPos = currentTarget();
    int32_t consign = initialPos + 10000;

    setTarget( consign );

    while( currentTarget() < ( consign - THRESHOLD ) )
    {
        QThread::msleep( 10 );
    }

    if( lastReference() != initialRef )
    {
        _reference = lastReference();
    }
    else
    {
        return false;
    }

    // Enable module
    setEnable( true );
    return true;
}

void Carrousel::setPeriod( int32_t period )
{
    _periodReg->write( period );
}

void Carrousel::setEnable( bool enabled )
{
   if( enabled )
   {
       _enableReg->write( 0x01 );
   }
   else
   {
       _enableReg->write( 0x00 );
   }
}

void Carrousel::setGainKp( float kp )
{
    _kpReg->write( kp );
}

void Carrousel::setGainKi( float ki )
{
    _kpReg->write( ki );
}

void Carrousel::setGainKd( float kd )
{
    _kpReg->write( kd );
}

void Carrousel::setSpeed( float speed )
{
    _speedReg->write( speed );
}

void Carrousel::setAcceleration( float acc )
{
    _accReg->write( acc );
}

void Carrousel::setOutputSaturation( uint32_t sat )
{
    _outputSatReg->write( sat );
}

void Carrousel::setTarget( int32_t position )
{
    _currentTargetReg->write( position );
}

int32_t Carrousel::period() const
{
    return _periodReg->read< int32_t >();
}

float Carrousel::kp() const
{
    return _kpReg->read< float >();
}

float Carrousel::ki() const
{
    return _kiReg->read< float >();
}

float Carrousel::kd() const
{
    return _kdReg->read< float >();
}

float Carrousel::speed() const
{
    _speedReg->read< float >();
}

float Carrousel::acceleration() const
{
    _accReg->read< float >();
}

uint32_t Carrousel::outputSaturation() const
{
    _outputSatReg->read< uint32_t >();
}

int32_t Carrousel::currentMeasure() const
{
    _currentMeasureReg->read< int32_t >();
}

int32_t Carrousel::currentTarget() const
{
    _currentTargetReg->read< int32_t >();
}

int32_t Carrousel::lastReference() const
{
    _lastRefReg->read< int32_t >();
}

int32_t Carrousel::reference() const
{
    return _reference;
}
