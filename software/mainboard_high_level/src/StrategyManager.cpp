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
    const uint width = 300; //300cm
    const uint height = 200; //200cm
    const uint scale = 1;

    _gameMap->setScale( scale );
    _gameMap->setMap( width, height );

    _gameMap->addElement( 20, 60, 6 );
    _gameMap->addElement( 280, 60, 6 );

    _gameMap->addElement( 100, 60, 6 );
    _gameMap->addElement( 200, 60, 6 );

    _gameMap->addElement( 50, 110, 6 );
    _gameMap->addElement( 250, 110, 6 );

    _gameMap->addElement( 50, 110, 6 );
    _gameMap->addElement( 250, 110, 6 );

    _gameMap->addElement( 90, 140, 6 );
    _gameMap->addElement( 210, 140, 6 );

    _gameMap->addElement( 80, 185, 6 );
    _gameMap->addElement( 220, 185, 6 );

    _gameMap->setStart( 0, 90 );
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
