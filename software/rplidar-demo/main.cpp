// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include <WestBot/RPLidar/RPLidar.hpp>

using namespace WestBot::RPLidar;

int main(int argc, char *argv[])
{
    QCoreApplication app( argc, argv );

    RPLidar lidar( "/dev/ttyUSB0" );

    if( ! lidar.connect() )
    {
        qWarning() << "Cannot connect to RPLidar";
        return EXIT_FAILURE;
    }
    qDebug() << "Connected";

    lidar.startMotor();
    qDebug() << "Start motor";
    QThread::sleep( 5 );

    lidar.stopMotor();
    qDebug() << "Stop motor";

    lidar.disconnect();
    qDebug() << "Disconnect";

    return app.exec();
}

