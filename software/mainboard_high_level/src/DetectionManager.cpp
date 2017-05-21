// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>
#include <QTimer>
#include <QThread>

#include <WestBot/DetectionManager.hpp>

using namespace WestBot;

DetectionManager::DetectionManager( const QString& name, QObject* parent )
    : QObject( parent )
    , _name( name )
    , _opponentDetected( false )
{
    _eventTimer = new QTimer( this );

    connect(
        _eventTimer,
        & QTimer::timeout,
        this,
        & DetectionManager::check );
}

void DetectionManager::init( Hal& hal )
{
    _distanceSensor = std::make_shared< ItemRegister >( hal._distanceSensor );

    while( _distanceSensor->read< uint8_t >() == 0 )
    {
        QThread::msleep( 10 );
        qDebug() << "Wait distance sensor to be ready...";
    }

    _eventTimer->start( 100 );

    check();
}

// Private methods
void DetectionManager::check()
{
    const uint32_t distanceValue = _distanceSensor->read< uint32_t >();

    if( ! _opponentDetected && distanceValue < 255 )
    {
        emit opponentDetected( true );
        _opponentDetected = true;
    }
    else if( _opponentDetected && distanceValue == 255 )
    {
        emit opponentDetected( false );
        _opponentDetected = false;
    }

    //qDebug() << "Proximity sensor value:" << distanceValue;
}
