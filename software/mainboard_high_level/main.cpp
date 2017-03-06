// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <stdlib.h>

#include <memory>

#include <QCoreApplication>
#include <QTimer>

#include "GameManager.hpp"
#include "Led.hpp"
#include "MemoryManager.hpp"

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Manage the LW_BRIDGE FOR US
    MemoryManager manager;

    // La tirette
    Button::Ptr fireStarter =
        std::make_shared< Button >( manager, "fireStarter" );

    // Create a simple manager (Led peripheral)
    Led ledManager( manager );

    // General manager for strat.
    // Manage a state machine to process game actions.
    GameManager gameManager( fireStarter );

    // Simply turn on LED_1
    ledManager.turnOn( 1 );

    return app.exec();
}
