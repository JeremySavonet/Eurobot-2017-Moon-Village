// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QDebug>

#include <WestBot/AStar/Utils.hpp>

#include <WestBot/StrategyManager.hpp>

using namespace WestBot;

StrategyManager::StrategyManager(
    SystemManager& systemManager,
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
{
    _gameMap = std::make_shared< GameMap >();

    connect(
        & _systemManager,
        & SystemManager::readyForWar,
        this,
        [ this ]()
        {
            qDebug() << "Ready to start strategy thread...";
        } );
}


void StrategyManager::buildMap()
{
    const uint width = 25;
    const uint height = 25;
    const uint scale = 1;

    _gameMap->setScale( scale );
    _gameMap->setMap( width, height );
}

void StrategyManager::buildPath()
{
    _gameMap->showPath();
}

void StrategyManager::toEuclidean()
{
    qDebug() << "Changing heuristic to euclidian";
    _gameMap->setHeuristics( AStar::AStarHeuristics::euclidean );
}

void StrategyManager::toManhattan()
{
    qDebug() << "Changing heuristic to manhattan";
    _gameMap->setHeuristics( AStar::AStarHeuristics::manhattan );
}


void StrategyManager::dumpMap()
{    
    _gameMap->dumpMap();
}
