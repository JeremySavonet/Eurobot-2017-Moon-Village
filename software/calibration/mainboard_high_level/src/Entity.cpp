// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <WestBot/Entity.hpp>

using namespace WestBot;

Entity::Entity( int x, int y, Entity::Type type, QObject* parent )
    : QObject( parent )
    , _type( type )
    , _position( x, y )
{
}

Entity::Type Entity::type() const
{
    return _type;
}

void Entity::setType( Entity::Type type )
{
    if( type != _type )
    {
        _type = type;
        emit updated();
    }
}

int Entity::cost() const
{
    // TODO: XXX
    return 0;
}

const QPoint& Entity::position() const
{
    return _position;
}

void Entity::setPosition( int x, int y )
{
    if( _position.x() != x )
    {
        _position.setX( x );
        emit updated();
    }

    if( _position.y() != y )
    {
        _position.setY( y );
        emit updated();
    }
}
