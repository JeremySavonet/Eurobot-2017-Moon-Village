// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_CONFIGURATIONTCPSERVER_HPP_
#define WESTBOT_CONFIGURATIONTCPSERVER_HPP_

#include <memory>

#include <QHash>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "Configuration.hpp"

namespace WestBot {

class ConfigurationTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    using SocketPtr = std::shared_ptr< QTcpSocket >;

    ConfigurationTcpServer( Configuration& configurationManager,
                            QObject* parent = nullptr );

    void disconnectClient( const SocketPtr& socket );

signals:
    void error( QTcpSocket::SocketError socketError );

protected:
    void incomingConnection( qintptr socketDescriptor ) override;

private:
    void parseData( const SocketPtr& socket );
    void sendConfiguration( const SocketPtr& socket );

private:
    QHash< QObject*, SocketPtr > _clients;
    Configuration& _configurationManager;
};

}

#endif // WESTBOT_CONFIGURATIONTCPSERVER_HPP_
