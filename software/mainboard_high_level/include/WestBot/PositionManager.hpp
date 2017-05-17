// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_POSITIONMANAGER_HPP_
#define WESTBOT_POSITIONMANAGER_HPP_

#include <QObject>

#include <WestBot/RPLidar/RPLidar.hpp>

class QTimer;

namespace WestBot {

class PositionManager : public QObject
{
    Q_OBJECT

public:
    PositionManager( RPLidar::RPLidar& lidar, QObject* parent = nullptr );
    ~PositionManager() override = default;

    void init();

    // Helpers for position
    void capture();
    void display();
    void recallage();

signals:
    void positionUpdated( int theta, int x, int y );

private:
    void process();

    RPLidar::RPLidar& _lidar;
    QTimer* _recallageTimer;
};

}

#endif // WESTBOT_POSITIONMANAGER_HPP_
