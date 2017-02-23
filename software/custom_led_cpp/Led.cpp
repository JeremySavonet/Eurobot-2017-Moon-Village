// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "hps_0.h"

#include "Led.hpp"

using namespace WestBot;

Led::Led( MemoryManager& manager )
    : _memoryManager( manager )
{
    _ledMap = _memoryManager.mapModuleToMemory( CUSTOM_LEDS_0_BASE );
}

Led::~Led()
{
    reset();
}

void Led::turnOn( int ledId )
{
    if( ledId == 1 )
    {
        *_ledMap = 0x02;
    }
    else
    {
        *_ledMap = 0xFF;
    }
}

void Led::turnOff( int ledId )
{
    if( ledId == 1 )
    {
        *_ledMap = 0x00;
    }
    else
    {
        *_ledMap = 0xFF;
    }
}

void Led::reset()
{
    // Turn all the LEDS off
    *_ledMap = 0x00;
}
