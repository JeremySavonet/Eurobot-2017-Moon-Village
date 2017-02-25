// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <stdlib.h>

#include <QCoreApplication>
#include <QTimer>

#include "Led.hpp"
#include "MemoryManager.hpp"

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Timer pour le match
    QTimer* timer = new QTimer();
    timer->setSingleShot( true ); // In single shot mode. Need to be re-armed each time.

    // Manage the LW_BRIDGE FOR US
    MemoryManager manager;

    // Create a simple manager (Led peripheral)
    const Led::Ptr ledManager =
        std::make_shared< Led >( manager );

    // Simply turn on LED_1
    ledManager->turnOn( 1 );

    QObject::connect(
        timer,
        & QTimer::timeout,
        & app,
        [ ledManager ]() mutable
        {
            ledManager->turnOff( 1 );
        } );

    timer->start( 90 * 1000 );

    return app.exec();
}
