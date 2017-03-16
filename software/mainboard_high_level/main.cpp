// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <stdlib.h>

#include <QMap>
#include <QDebug>
#include <QThread>

#include <QCoreApplication>

#include "hps_arm.h" // For our base address

#include "Layer1.hpp"

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
    LayerManager< Layer1_t > layerManager( manager, PIO_N_LAYER1_BASE );

    // Then configure:
    layerManager.write( 1 * 4, 32, 0xffffffff );
    qDebug() << "READ REG" << 1 << ":" << QString::number( layerManager.read( 1 * 4, 32 ), 16 );

    layerManager.write( 10 * 4, 32, 0x01010101 );
    qDebug() << "READ REG" << 10 << ":" << QString::number( layerManager.read( 10 * 4, 32 ), 16 );


    while( 1 )
    {
        layerManager.write( 9 * 4, 32, 0x01010101 );
        QThread::msleep( 1000 );
        layerManager.write( 9 * 4, 32, 0x00000000 );
        QThread::msleep( 1000 );

        qDebug() << "READ SW - REG 7:" << layerManager.read( 7 * 4, 32 );
    }

    /*layerManager.write( 9 * 4, 32, 0x01010101 );
    layerManager.write( 8 * 4, 32, 0x01010101 );

    for( int i = 0; i < 64; i++ )
    {
        qDebug() << "READ REG" << i << ":" << QString::number( layerManager.read( i * 4, 32 ), 16 );
    }

    //qDebug() << "READ REG" << 10 << ":" << QString::number( layerManager.read( 10 * 4, 32 ), 16 );

    // Handle button changes
    while( 1 )
    {
        qDebug() << "READ SW - REG 7:" << layerManager.read( 7 * 4, 32 );
        QThread::msleep( 100 );
    }
*/
    return app.exec();
}
