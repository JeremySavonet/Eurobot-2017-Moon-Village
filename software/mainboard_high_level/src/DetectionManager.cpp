// Copyright (c) 2017 All Rights Reserved WestBot

#include <QCoreApplication>
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

bool DetectionManager::init( Hal& hal )
{
    bool isTimeout = false;

    QTimer timeout;
    timeout.setSingleShot( true );
    timeout.start( 1000 );
    QObject::connect(
        & timeout,
        & QTimer::timeout,
        [ &isTimeout ]()
        {
            isTimeout = true;
        } );

    _distanceSensor = std::make_shared< ItemRegister >( hal._distanceSensor );

    while( ! isTimeout && _distanceSensor->read< uint8_t >() == 0 )
    {
        if( isTimeout )
        {
            return false;
        }

        QThread::msleep( 10 );
        QCoreApplication::processEvents();
    }

    _eventTimer->start( 10 );

    check();

    return true;
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

    qDebug() << ">>>>>>>>>>>>>>>VALUE" << distanceValue;
}
