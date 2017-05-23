// Copyright (c) 2017 All Rights Reserved WestBot

#include <QThread>

#include <WestBot/WaitAction.hpp>

using namespace WestBot;

WaitAction::WaitAction( int waitMs )
    : Action()
    , _waitMs( waitMs )
{
}

void WaitAction::execute()
{
    QThread::msleep( _waitMs );

    emit complete();
}