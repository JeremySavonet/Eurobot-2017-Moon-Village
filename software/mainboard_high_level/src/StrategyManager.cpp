// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QDebug>

#include <WestBot/StrategyManager.hpp>

using namespace WestBot;

StrategyManager::StrategyManager(
    SystemManager& systemManager,
    TrajectoryManager& trajectoryManager,
    Carrousel& carrousel,
    Servo& armRight,
    Servo& armLeft,
    Servo& ejector,
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
    , _trajectoryManager( trajectoryManager )
    , _carrousel( carrousel )
    , _armRight( armRight )
    , _armLeft( armLeft )
    , _ejector( ejector )
{
    connect(
        & _systemManager,
        & SystemManager::doStrat,
        this,
        [ this ]( const Color& color )
        {
            qDebug() << "Ready to start strategy thread...";
            doStrat( color );
        } );

    connect(
        & _systemManager,
        & SystemManager::hardStop,
        this,
        [ this ]()
        {
            qDebug() << "Hardstop pushed";
        } );

    connect(
        & _systemManager,
        & SystemManager::doFunnyAction,
        this,
        [ this ]()
        {
            qDebug() << "Funny action time...";
        } );

    connect(
        & _systemManager,
        & SystemManager::stop,
        this,
        [ this ]()
        {
            qDebug() << "Stop the game: Rearming strat";
        } );
}

// Private methods

void StrategyManager::doStrat( const Color& color )
{
    qDebug() << "Do strat for color:" << color;

    // TODO: XXX
}
