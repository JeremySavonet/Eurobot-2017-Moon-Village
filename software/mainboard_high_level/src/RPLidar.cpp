// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>
#include <QSerialPort>
#include <QString>
#include <QTimer>
#include <QThread>

#include <WestBot/RPLidar.hpp>

using namespace WestBot;

RPLidar::RPLidar( const QString& port )
    : _tty( new QSerialPort( port ) )
{
}

bool RPLidar::startScan( int timeoutMs )
{
    bool isTimeout = false;

    QTimer timeout;
    timeout.setSingleShot( true );
    QObject::connect(
        & timeout,
        & QTimer::timeout,
        [ & isTimeout ]()
        {
            isTimeout = true;
        } );

    timeout.start( timeoutMs );

    while( ! isTimeout && ! _tty->bytesAvailable() )
    {
        QThread::msleep( 10 );
    }

    timeout.stop();

    if( isTimeout )
    {
        qWarning() << "Could not start scan: Command timeout";
        return false;
    }

    return true;
}

bool RPLidar::connect()
{
    if( ! _tty->setBaudRate( QSerialPort::Baud115200 ) ||
        ! _tty->setDataBits( QSerialPort::Data8 ) ||
        ! _tty->setParity( QSerialPort::NoParity ) ||
        ! _tty->setStopBits( QSerialPort::OneStop ) ||
        ! _tty->setFlowControl( QSerialPort::NoFlowControl ) )
    {
        qWarning() << "Failed to set port parameters";
        return false;
    }

    if( ! _tty->open( QIODevice::ReadWrite ) )
    {
        qDebug() << "Failed to open RPLidar port";
        return false;
    }

    return true;
}

void RPLidar::disconnect()
{
    _tty->close();
}

void RPLidar::flushScanData()
{
    _tty->readAll();
}

bool RPLidar::grabScanData( double* distance, double* angle, int* len )
{
    const qint64 bytesLen = _tty->bytesAvailable();

    if( bytesLen == 0 )
    {
        qWarning() << "Error: No bytes available";
        return false;
    }

    uint8_t buffer[ bytesLen ];
    _tty->read( ( char *) buffer, *len );

    return true;
}

