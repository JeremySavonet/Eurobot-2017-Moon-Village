// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "hps_arm.h"

#include "Button.hpp"

using namespace WestBot;

Button::Button( MemoryManager& manager,
                const QString& name,
                QObject* parent )
    : QObject( parent )
    , _memoryManager( manager )
    , _name( name )
    , _isPressed( false )
{
    //_buttonMap = _memoryManager.mapModuleToMemory( CUSTOM_LEDS_0_BASE );
}

Button::~Button()
{
    reset();
}

const QString& Button::name() const
{
    return _name;
}

void Button::reset()
{
    *_buttonMap = 0x00;
}

// Protected methods

void Button::handleEvents()
{
    // Polling for button events
    while( true )
    {
        if( *_buttonMap == 0x00 && _isPressed )
        {
            _isPressed = false;
            emit released();
        }
        else if( *_buttonMap == 0x01 && ! _isPressed )
        {
            _isPressed = true;
            emit pressed();
        }
    }
}
