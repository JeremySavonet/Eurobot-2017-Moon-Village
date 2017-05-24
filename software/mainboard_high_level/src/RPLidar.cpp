// Copyright (c) 2017 All Rights Reserved WestBot

#include <QCoreApplication>
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

void RPLidar::init( Hal& hal )
{
	hal._motor5Override.write( 1 );
	hal._motor5Value.write( 25000 / 2);
	QThread::msleep( 2000 );
}

void RPLidar::disconnect()
{
    _tty->close();
}

char RPLidar::checksum( char buf[], int bufSize )
{
	uint8_t chk = 0;
	for(int i=0; i<bufSize; i++)
		chk ^= (uint8_t)buf[i];
	return chk;
}

bool RPLidar::startScan( int timeoutMs )
{
	qDebug() << "telem start";

	QTimer timeout;
	timeout.setSingleShot( true );

	timeoutMs = 3000;

	timeout.start( timeoutMs );
	qDebug() << "timeoutMs " << timeoutMs;

	int status = 0;
	while( timeout.remainingTime()>0 )
	{
		QCoreApplication::processEvents();

		switch(status) {
			case 0: {
				const char buf[] = {0xA5,0x25};
				_tty->write(buf,sizeof(buf));
				QCoreApplication::processEvents();
				QThread::msleep( 1000 );
				status++;
				qDebug() << "telem stop done";
				break;
			}
			case 1: {
				char buf[] = {0xA5,0x52,0};
				buf[sizeof(buf)-1] = checksum(buf,sizeof(buf));
				_tty->readAll();
				_tty->write(buf,sizeof(buf));
				QCoreApplication::processEvents();
				QThread::msleep( 100 );
				QCoreApplication::processEvents();
				unsigned int len = _tty->bytesAvailable();
				if(len==0)
					break;
				char RESP[] = {165, 90, 3, 0, 0, 0, 6};
				if(len>sizeof(RESP))
					len = sizeof(RESP);
				if(len!=sizeof(RESP))
					break;
				char resp[len];
				_tty->read(resp,len);
				if(strcmp(resp,RESP)!=0)
					break;
				status++;
				qDebug() << "telem health done";
				break;
			}
			case 2: {
				char buf[] = {0xA5,0x82,5,0,0,0,0,0,0};
				buf[sizeof(buf)-1] = checksum(buf,sizeof(buf));
				_tty->readAll();
				_tty->write(buf,sizeof(buf));
				QCoreApplication::processEvents();
				QThread::msleep( 50 );
				QCoreApplication::processEvents();
				unsigned int len = _tty->bytesAvailable();
				if(len==0)
					break;
				char RESP[] = {165, 90, 84, 0, 0, 64, 130};
				if(len>sizeof(RESP))
					len = sizeof(RESP);
				if(len!=sizeof(RESP))
					break;
				char resp[len];
				_tty->read(resp,len);
				if(strcmp(resp,RESP)!=0)
					break;
				status++;
				qDebug() << "telem start done";
				break;
			}
			case 3: {
				QThread::msleep( 1000 );
				QCoreApplication::processEvents();
				int len = _tty->bytesAvailable();
				qDebug() << "len" << len;
				timeout.stop();
				if(len<100)
					return false;
				qDebug() << "telem init done";
				return true;
			}
			default: {
				status = 0;
			}
		}
	}

	timeout.stop();

	qWarning() << "Could not start scan: Command timeout";
	return false;
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




