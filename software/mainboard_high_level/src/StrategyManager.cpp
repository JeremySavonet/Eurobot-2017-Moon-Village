// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QDebug>

#include <WestBot/StrategyManager.hpp>

using namespace WestBot;

StrategyManager::StrategyManager(
    SystemManager& systemManager,
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
{
    connect(
        & _systemManager,
        & SystemManager::readyForWar,
        this,
        [ this ]()
        {
            qDebug() << "Ready to start strategy thread...";
        } );
}
