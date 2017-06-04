// Copyright (c) 2017 All Rights Reserved WestBot

#include <WestBot/ColorCheckerAction.hpp>

using namespace WestBot;

ColorCheckerAction::ColorCheckerAction(
    ColorSensor& colorSensor,
    Type type )
    : _colorSensor( colorSensor )
    , _type( type )
{

}

void ColorCheckerAction::execute()
{
    switch( _type )
    {
    case Type::EJECT_GOOD:
    {
        _colorSensor.sensorCheck();
    }
        break;
    }
}
