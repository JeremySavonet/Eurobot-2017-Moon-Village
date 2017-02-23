// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>
#include <QTimer>

#include "GameTimer.hpp"

using namespace WestBot;

GameTimer::GameTimer( int duration, QObject* parent )
    : QObject( parent )
    , _duration( duration )
{
    _timer = new QTimer( this );
    _timer->setSingleShot( true );

    connect(
        _timer,
        & QTimer::timeout,
        this,
        & GameTimer::endGame );
}

void GameTimer::startGame()
{
    _timer->start( 1000 );
}

void GameTimer::stopGame()
{
    _timer->stop();
}

void GameTimer::endGame()
{
    qDebug() << "<<<<<<<<<<<<<<<<<<< FUCK";
    emit gameEnded();
}
