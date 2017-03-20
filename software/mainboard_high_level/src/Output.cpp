// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <WestBot/Output.hpp>

using namespace WestBot;

Output::Output(
    const ItemRegister& outputRegister,
    OutputType type,
    const QString& name )
    : _outputRegister( outputRegister )
    , _type( type )
    , _name( name )
    , _digitalValue( Output::Value::OFF )
{
    digitalRead();
}

const QString& Output::name() const
{
    return _name;
}

void Output::digitalWrite( Value val )
{
    if( val == Output::Value::ON )
    {
        _outputRegister.write( 0x01 );
    }
    else
    {
        _outputRegister.write( 0x00 );
    }
}

Output::Value Output::digitalRead()
{
    if( _outputRegister.read() == 0x01 )
    {
        _digitalValue = Output::Value::ON;
    }
    else
    {
        _digitalValue = Output::Value::OFF;
    }
}

QDebug operator<<( QDebug debug, Output::Value value )
{
    switch( value )
    {
    case Output::Value::OFF:
        debug << "Output::OFF";
        break;

    case Output::Value::ON:
        debug << "Output::ON";
        break;
    }
    return debug;
}
