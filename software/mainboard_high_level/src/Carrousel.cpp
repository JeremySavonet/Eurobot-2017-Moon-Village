// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>

#include <WestBot/Carrousel.hpp>

using namespace WestBot;

namespace
{
    const int THRESHOLD = 20;
}

Carrousel::Carrousel( Hal& hal, QObject* parent )
    : QObject( parent )
    , _hal( hal )
    , _reference( 0 )
    , _nbTickPerTour( 0 )
    , _positionTimeout( false )
{
    _positionTimer = new QTimer();

    connect(
        _positionTimer,
        & QTimer::timeout,
        this,
        [ this ]()
        {
            qDebug()
                << QTime::currentTime().toString()
                << "Cannot reach position. Maybe it's blocked=====> drop!";
            _positionTimeout = true;
            setEnable( false );
        } );
}

bool Carrousel::init()
{
    // Disable the module first
    setEnable( false );

    setOverride( true );

    // Set PID gains
    setGainKp( 100.0 );
    setGainKi( 0.0 );
    setGainKd( 0.0 );

    setSpeed( 10.0 );
    setAcceleration( 0.001 );
    setOutputSaturation( 15000 );

    int32_t initialRef = lastReference();
    int32_t initialPos = currentPosition();
    int32_t consign = initialPos + 10000;

    setTarget( initialPos );

    // Enable module
    setEnable( true );

    setTarget( consign );

    // Find first ref
    while( abs( currentPosition() - consign ) >= THRESHOLD )
    {
        QThread::msleep( 10 );
    }

    int32_t firstRef = 0;
    if( lastReference() != initialRef )
    {
        firstRef = lastReference();
    }
    else
    {
        return false;
    }

    consign = currentPosition() + 10000;

    // Set new target
    setTarget( consign );

    // Find ref
    while( abs( currentPosition() - consign ) >= THRESHOLD )
    {
        QThread::msleep( 10 );
    }

    if( lastReference() != firstRef )
    {
        _reference = lastReference();

    }
    else
    {
        return false;
    }

    _nbTickPerTour = _reference - firstRef;

    qDebug()
        << QTime::currentTime().toString()
        << "Carrousel module initialized. Nb tick:"
        << _nbTickPerTour;

    return true;
}

//
// Public methods
//
float Carrousel::position()
{
    float pos = ( ( ( currentPosition() - reference() ) % _nbTickPerTour ) /
             ( float ) _nbTickPerTour ) * 6.0f;

    if( pos < 0.0 )
    {
        pos += 6.0;

    }

    return pos;
}

void Carrousel::setPosition( float targetPos )
{
    _positionTimeout = false;
    //_positionTimer->start( 1000 ); // timeout of 1s. When reached drop the command.

    float diff = targetPos - position();
    if( diff > 3.0 )
    {
        diff -= 6.0;
    }

    if( diff < -3.0 )
    {
        diff += 6.0;
    }

    int32_t posInTick =
        ( int32_t ) ( diff * ( ( float ) _nbTickPerTour / 6.0f ) );

    setTarget( currentPosition() + posInTick );

   /* while( currentPosition() < targetPos && ! _positionTimeout )
    {
        QThread::msleep( 10 );
        qDebug()
            << "Wait carrousel to be in pos:" << targetPos
            << "current pos is:" << currentPosition();
        QCoreApplication::processEvents();
    }

    // Reset and enable module after timeout.
    if( _positionTimeout )
    {
        _positionTimeout = false;
        setEnable( true );
    }
    */
}

void Carrousel::enable( bool enable )
{
    setEnable( enable );
}

//
// Private methods
//
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

void Carrousel::setOverride( bool enable )
{
    _hal._carrouselOverride.write( enable ? 1 : 0 );
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
