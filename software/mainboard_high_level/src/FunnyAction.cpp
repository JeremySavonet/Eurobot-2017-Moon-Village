// Copyright (c) 2017 All Rights Reserved WestBot

// Author: B. Vincent
// Author: J.Savonet

#include <QDebug>
#include <QProcess>
#include <QStringList>

#include <WestBot/FunnyAction.hpp>

using namespace WestBot;

FunnyAction::FunnyAction()
    : Action()
    , _process( new QProcess( this ) )
{
}

void FunnyAction::execute()
{
    qDebug() << "Execute funny action";

    connect(
        _process,
        SIGNAL( finished( int ) ),
        this,
        SLOT( complete() ) );

    _process->start( "/bin/sh", QStringList() << "/root/scripts/replayFunnyAction.sh" );
}
