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
    _hal._carrouselPidPeriod.write( period );
}

void Carrousel::setEnable( bool enabled )
{
   if( enabled )
   {
       _hal._carrouselEnable.write( 1 );
   }
   else
   {
       _hal._carrouselEnable.write( 0 );
   }
}

void Carrousel::setGainKp( float kp )
{
    _hal._carrouselPidKp.write( kp );
}

void Carrousel::setGainKi( float ki )
{
    _hal._carrouselPidKi.write( ki );
}

void Carrousel::setGainKd( float kd )
{
    _hal._carrouselPidKd.write( kd );
}

void Carrousel::setSpeed( float speed )
{
    _hal._carrouselSpeed.write( speed );
}

void Carrousel::setAcceleration( float acc )
{
    _hal._carrouselAcc.write( acc );
}

void Carrousel::setOutputSaturation( uint32_t sat )
{
    _hal._carrouselOutputSaturation.write( sat );
}

void Carrousel::setTarget( int32_t position )
{
    _hal._carrouselTarget.write( position );
}

int32_t Carrousel::period() const
{
    return _hal._carrouselPidPeriod.read< int32_t >();
}

float Carrousel::kp() const
{
    return _hal._carrouselPidKp.read< float >();
}

float Carrousel::ki() const
{
    return _hal._carrouselPidKi.read< float >();
}

float Carrousel::kd() const
{
    return _hal._carrouselPidKd.read< float >();
}

float Carrousel::speed() const
{
    return _hal._carrouselSpeed.read< float >();
}

float Carrousel::acceleration() const
{
    return _hal._carrouselAcc.read< float >();
}

uint32_t Carrousel::outputSaturation() const
{
    return _hal._carrouselOutputSaturation.read< uint32_t >();
}

int32_t Carrousel::currentPosition() const
{
    return _hal._carrouselPosition.read< int32_t >();
}

int32_t Carrousel::currentTarget() const
{
    return _hal._carrouselTarget.read< int32_t >();
}

int32_t Carrousel::lastReference() const
{
    return _hal._carrouselLastReference.read< int32_t >();
}

int32_t Carrousel::reference() const
{
    return _reference;
}
