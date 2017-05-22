// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QTimer>

#include <WestBot/PositionManager.hpp>

using namespace WestBot;

PositionManager::PositionManager( RPLidar::RPLidar& lidar, QObject* parent )
    : QObject( parent )
    , _lidar( lidar )
{
    _recallageTimer = new QTimer( this );

    connect(
        _recallageTimer,
        & QTimer::timeout,
        this,
        & PositionManager::process );
}

bool PositionManager::init()
{
    return true;
}

void PositionManager::capture()
{

}

void PositionManager::display()
{

}

void PositionManager::recallage()
{

}

// Private methods
void PositionManager::process()
{

}
