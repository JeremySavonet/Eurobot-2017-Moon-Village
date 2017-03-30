// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include <Defines.hpp>

#include <WestBot/Configuration.hpp>
#include <WestBot/Hal.hpp>
#include <WestBot/SystemManager.hpp>

namespace
{
    WESTBOT_STR( CONFIGURATION_PATH, "config.ini" );
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
