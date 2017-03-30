// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QThread>

#include <Defines.hpp>

#include <WestBot/Configuration.hpp>
#include <WestBot/ConfigurationTcpServer.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

namespace
{
    WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );

    const int DEFAULT_PORT = 57497;
}

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Load the configuration manager
    Configuration configurationManager( CONFIGURATION_PATH );

    QObject::connect(
        & configurationManager,
        & Configuration::configurationChanged,
        [ &configurationManager ]()
        {
            qDebug() << "Update layers according to configuration changes";
            configurationManager.load();
        } );

    configurationManager.load();

    // Start the tcp configuration server
    ConfigurationTcpServer configurationServer( configurationManager );

    if( ! configurationServer.listen( QHostAddress::Any, DEFAULT_PORT ) )
    {
        qDebug()
            << "Unable to start the server:"
            << configurationServer.errorString();
        return -1;
    }

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
            << configurationServer.serverPort();
    }

    // Init the Hardware Abstraction Layer
    Hal hal;
    hal.init(); // Open the FPGA memory and load our module

    // Init the System manager
    SystemManager system( hal );
    system.init(); // Create the state machine

    // Here we are BITCHESSSSSS !!!
    system.start(); // Start the state machine

    while( 1 )
    {
        QThread::msleep( 100 ); // Idle
        QCoreApplication::processEvents();
    }

    return app.exec();
}
