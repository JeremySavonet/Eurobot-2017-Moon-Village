// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/Turbine.hpp>

using namespace WestBot;

Turbine::Turbine( const QString& name )
    : _name( name )
    , _value( 0 )
{
}

bool Turbine::attach( Hal& hal )
{
    _escEnable = std::make_shared< ItemRegister >( hal._esc0Enable );
    _escOverride = std::make_shared< ItemRegister >( hal._esc0Override );
    _escValue = std::make_shared< ItemRegister >( hal._esc0Value );

    _escValue->write( 0 );
    _escOverride->write( 0x01 );
    _escEnable->write( 0x1 );

    qDebug() << "Turbine is attached";
    return true;
}

void Turbine::reset()
{
    setValue( 0 );
    enable( false );
}

const QString& Turbine::name() const
{
    return _name;
}

void Turbine::enable( bool enable )
{
    if( enable )
    {
        _escEnable->write( 0x1 );
    }
    else
    {
        _escEnable->write( 0x0 );
    }
}

void Turbine::setValue( int16_t value )
{
    _escValue->write( value );
    _value = value;
}

int16_t Turbine::value() const
{
    return _value;
}
