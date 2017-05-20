// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QNetworkInterface>
#include <QString>
#include <QStringBuilder>
#include <QVariant>

#include <WestBot/ConfigurationTcpServer.hpp>

using namespace WestBot;

ConfigurationTcpServer::ConfigurationTcpServer(
    Configuration& configurationManager, QObject* parent )
    : QTcpServer( parent )
    , _configurationManager( configurationManager )
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
        & QTcpSocket::connected,
        this,
        [ this, tcpSocket ]()
        {
            sendConfiguration( tcpSocket );
        } );

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
        [ this, tcpSocket ]()
        {
            parseData( tcpSocket );
        } );

}

void ConfigurationTcpServer::disconnectClient( const SocketPtr& socket )
{
    _clients.remove( socket.get() );
}

void ConfigurationTcpServer::showConnectionInformation() const
{
    if( isListening() )
    {
        QString ipAddress;
        QList< QHostAddress > ipAddressesList = QNetworkInterface::allAddresses();

        // use the first non-localhost IPv4 address
        for( int i = 0; i < ipAddressesList.size(); ++i )
        {
            if( ipAddressesList.at( i ) != QHostAddress::LocalHost &&
                    ipAddressesList.at( i ).toIPv4Address() )
            {
                ipAddress = ipAddressesList.at( i ).toString();
                break;
            }
        }

        // if we did not find one, use IPv4 localhost
        if( ipAddress.isEmpty() )
            ipAddress = QHostAddress( QHostAddress::LocalHost).toString();
        {
            qDebug()
                << "The server is running on\n\nIP:" << ipAddress << "\nport:"
                << serverPort();
        }
    }
    else
    {
        qDebug() << "No client bound on server.";
    }
}

void ConfigurationTcpServer::parseData( const SocketPtr& socket )
{
    const QByteArray data = socket->readAll();
    QJsonDocument configuration = QJsonDocument::fromJson( data );

    if( configuration.isObject() )
    {
        parseConfiguration( configuration );
    }
    else
    {
        parseCliCommand( data );
    }
}

void ConfigurationTcpServer::parseConfiguration(
    const QJsonDocument& configuration )
{
    qDebug() << "Parse configuration:" << configuration;
}

void ConfigurationTcpServer::parseCliCommand( const QByteArray& command )
{
    qDebug() << "Parse cli command:" << command;
}

void ConfigurationTcpServer::sendConfiguration( const SocketPtr& socket )
{
    socket->write( "Connected to WestBot Server\r\n" );
    socket->flush();
    socket->waitForBytesWritten( 3000 );

//    const QHash< QString, QVariant > conf = _configurationManager.settings();
//    for( auto it = conf.constBegin(); it != conf.constEnd(); ++it )
//    {
//        QString message = "PARAM " % it.key() % ' ' % it.value().toString();
//        socket->write( message.toLatin1() );
//        socket->flush();

//        qDebug() << "Byte written:" << message.toLatin1();
//    }

//    qDebug() << "Send configuration done";
}
