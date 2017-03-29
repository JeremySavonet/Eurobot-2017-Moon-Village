// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>
#include <QString>

#include <WestBot/ConfigurationTcpServer.hpp>

using namespace WestBot;

ConfigurationTcpServer::ConfigurationTcpServer( QObject* parent )
    : QTcpServer( parent )
{
}

void ConfigurationTcpServer::incomingConnection( qintptr socketDescriptor )
{
    SocketPtr tcpSocket =
        std::make_shared< QTcpSocket >( nextPendingConnection() );

    qDebug() << "New incoming connection...";

    if( ! tcpSocket->setSocketDescriptor( socketDescriptor ) )
    {
        emit error( tcpSocket->error() );
        return;
    }

    _clients.insert( tcpSocket.get(), tcpSocket );

    connect(
        tcpSocket.get(),
        & QTcpSocket::disconnected,
        this,
        [ this, tcpSocket ]()
        {
            disconnectClient( tcpSocket );
        } );

    connect(
        tcpSocket.get(),
        & QTcpSocket::readyRead,
        this,
        & ConfigurationTcpServer::parseData );

    tcpSocket->write( "Connected to WestBot Server\r\n" );
    tcpSocket->flush();
    tcpSocket->waitForBytesWritten( 3000 );
}

void ConfigurationTcpServer::disconnectClient( const SocketPtr& socket )
{
    _clients.remove( socket.get() );
}

void ConfigurationTcpServer::parseData( const SocketPtr& socket )
{
    qDebug() << socket.readAll();
}
