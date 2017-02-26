// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "Action.hpp"

using namespace WestBot;

Action::Action( const QString& name, QObject* parent )
    : QObject( parent )
    , _name( name )
    , _state( Action::State::Pending )
{
}

const QString& Action::name() const
{
    return _name;
}

void Action::execute()
{
    // XXX: TODO
    emit complete();
}

void Action::skip()
{
    // XXX: TODO
    emit skipped();
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
