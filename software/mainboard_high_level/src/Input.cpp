// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QTimer>

#include <WestBot/Input.hpp>

using namespace WestBot;

namespace
{
    const int MASK_START_INPUT = 0x00000001;
    const int MASK_COLOR_INPUT = 0x00000100;
    const int MASK_STOP_INPUT = 0x00010000;
}

Input::Input(
    const ItemRegister& inputRegister,
    Input::InputType type,
    const QString& name )
    : _inputRegister( inputRegister )
    , _type( type )
    , _name( name )
    , _digitalValue( DigitalValue::OFF )
{
    _eventTimer = new QTimer( this );

    connect(
        _eventTimer,
        & QTimer::timeout,
        this,
        & Input::check );

    _eventTimer->start( 100 );

    check();
}

const QString& Input::name() const
{
    return _name;
}

DigitalValue Input::digitalRead()
{
    check();
    return _digitalValue;
}

void Input::check()
{
    DigitalValue tmpVal;

    switch( _type )
    {
    case Input::InputType::Start:
    {
        if( ( _inputRegister.read() & MASK_START_INPUT ) == MASK_START_INPUT )
        {
           tmpVal = DigitalValue::ON;
        }
        else
        {
            tmpVal = DigitalValue::OFF;
        }
    }
        break;

    case Input::InputType::Color:
    {
        if( ( _inputRegister.read() & MASK_COLOR_INPUT ) == MASK_COLOR_INPUT )
        {
            tmpVal = DigitalValue::ON;
        }
        else
        {
            tmpVal = DigitalValue::OFF;
        }
    }
        break;

    case Input::InputType::Stop:
    {
        if( ( _inputRegister.read() & MASK_STOP_INPUT ) == MASK_STOP_INPUT )
        {
            tmpVal = DigitalValue::ON;
        }
        else
        {
            tmpVal = DigitalValue::OFF;
        }
    }
        break;

    case Input::InputType::Unused:
    {
        if( ( _inputRegister.read() & MASK_STOP_INPUT ) == MASK_STOP_INPUT )
        {
            tmpVal = DigitalValue::ON;
        }
        else
        {
            tmpVal = DigitalValue::OFF;
        }
    }
        break;


    }

    if( tmpVal != _digitalValue )
    {
        _digitalValue = tmpVal;
        emit stateChanged( _digitalValue );
    }
}
