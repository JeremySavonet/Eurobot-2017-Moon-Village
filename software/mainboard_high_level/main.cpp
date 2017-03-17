// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <stdlib.h>

#include <QMap>
#include <QDebug>
#include <QThread>

#include <QCoreApplication>

#include "hps_arm.h" // For our base address

#include "Button.hpp"
#include "Layer1.hpp"
#include "Layer2.hpp"
#include "Layer3.hpp"
#include "LayerManager.hpp"
#include "MemoryManager.hpp"

#define ADD_REGISTER( name, layerRegister ) \
    {                                       \
        name, layerRegister                 \
    }

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    // Manage the LW_BRIDGE
    MemoryManager manager;

    // Create our layer manager
    LayerManager< Layer1_t > layer1Manager( manager, PIO_N_LAYER1_BASE );
    LayerManager< Layer2_t > layer2Manager( manager, PIO_N_LAYER2_BASE );
    LayerManager< Layer3_t > layer3Manager( manager, PIO_N_LAYER3_BASE );

    // Read layer 1 registers
    qDebug() << "Layer 1 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer1Manager.read( i * 4, 32 ), 16 );
    }

    // Read layer 2 registers
    qDebug() << "Layer 2 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer2Manager.read( i * 4, 32 ), 16 );
    }

    // Read layer 3 registers
    qDebug() << "Layer 3 registers:";
    qDebug() << "==================";

    for( int i = 0; i < 64; ++i )
    {
        qDebug()
            << "Read register" << i << ":"
            << QString::number( layer3Manager.read( i * 4, 32 ), 16 );
    }

    // ================= //
    // TEST API USE CASE //
    // ================= //
    Button button( manager, "Test" );

    connect(
        & button,
        & Button::handleEvents,
        this,
        notifyButtonChanged );

    notifyButtonChanged( type )
    {
        if( type == AU )
        {

        }
        else
        {

        }
        ...
    }

    // DO NOTHING
    while( 1 )
    {
        QThread::msleep( 500 );
    }

    return app.exec();
}
