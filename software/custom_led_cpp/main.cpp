// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QThread>

#include "Led.hpp"
#include "MemoryManager.hpp"

using namespace WestBot;

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    MemoryManager manager;
    Led led( manager );

    while( true )
    {
        led.turnOn( 1 );
        QThread::msleep( 250 );
        led.turnOff( 1 );
        QThread::msleep( 250 );
    }

    return app.exec();
}
