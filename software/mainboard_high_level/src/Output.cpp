// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <WestBot/Output.hpp>

using namespace WestBot;

Output::Output( const ItemRegister& outputRegister, const QString& name )
    : _outputRegister( outputRegister )
    , _name( name )
    , _digitalValue( DigitalValue::OFF )
{
    digitalRead();
}

const QString& Output::name() const
{
    return _name;
}

void Output::digitalWrite( DigitalValue val )
{
    if( val == DigitalValue::ON )
    {
        _outputRegister.write( 0x01 );
    }
    else
    {
        _outputRegister.write( 0x00 );
    }
}

DigitalValue Output::digitalRead()
{
    if( _outputRegister.read< uint32_t >() == 0x01 )
    {
        _digitalValue = DigitalValue::ON;
    }
    else
    {
        _digitalValue = DigitalValue::OFF;
    }

    return _digitalValue;
}
