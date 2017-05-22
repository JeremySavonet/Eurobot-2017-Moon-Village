// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>
#include <QTimer>

#include <WestBot/Action.hpp>

using namespace WestBot;

Action::Action( QObject* parent )
    : QObject( parent )
    , _state( Action::State::Pending )
{
    _timeout = new QTimer( this );

    connect(
        _timeout,
        & QTimer::timeout,
        this,
        [ this ]()
        {
            qDebug() << "Action timed out";
        } );
}

Action::State Action::state() const
{
    return _state;
}

void Action::setState( Action::State state )
{
    if( _state != state )
    {
        _state = state;
        emit stateChanged();
    }
}

bool Action::hasError() const
{
    return _state == Action::State::InError;
}
