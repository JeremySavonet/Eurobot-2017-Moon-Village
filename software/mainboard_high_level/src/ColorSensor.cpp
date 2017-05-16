// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>
#include <QThread>

#include <WestBot/ColorSensor.hpp>

using namespace WestBot;

namespace
{
    const int COLOR_BLUE_THRESHOLD = 100;
    const int COLOR_YELLOW_THRESHOLD = 100;
}

ColorSensor::ColorSensor( const QString& name )
    : _name( name )
    , _sensor( nullptr )
    , _colorTarget( Color::Unknown )
{
}

bool ColorSensor::attach( Hal& hal,
                          uint8_t sensorId,
                          Color colorTarget )
{
    switch( sensorId )
    {
    case 0:
        _sensor = std::make_shared< ItemRegister >( hal._s0 ); // TODO: Change
        break;

    default:
        qDebug() << "Not a valid color sensor id to attach.";
        return false;
    }

    _colorTarget = colorTarget;

    return true;
}

const QString& ColorSensor::name() const
{
    return _name;
}

bool ColorSensor::isInRange()
{
    if( Color::Unknown == _colorTarget )
    {
        qWarning() << "No color target defined";
        return false;
    }

    // TODO: read register, compare to threashold and check with target color
    return true;
}

Color ColorSensor::sensorCheck()
{
    // TODO: read register and compare to threshold
    return Color::Blue;
}

bool ColorSensor::isAttached() const
{
    // TODO: Change
    return _sensor->read< uint8_t >() == 0x03 ? true : false;
}
