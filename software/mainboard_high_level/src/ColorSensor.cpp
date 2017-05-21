// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>
#include <QThread>

#include <WestBot/ColorSensor.hpp>

using namespace WestBot;

namespace
{
    const uint16_t COLOR_RED_THRESHOLD = 100;
    const uint16_t COLOR_GREEN_THRESHOLD = 100;
    const uint16_t COLOR_BLUE_THRESHOLD = 100;
    const uint16_t COLOR_CLEAR_THRESHOLD = 100;
}

ColorSensor::ColorSensor( const QString& name )
    : _name( name )
    , _sensorValid( nullptr )
    , _sensorRed( nullptr )
    , _sensorGreen( nullptr )
    , _sensorBlue( nullptr )
    , _sensorClear( nullptr )
    , _colorTarget( Color::Unknown )
    , _isAttached( false )
{

}

bool ColorSensor::attach( Hal& hal )
{
    if( ! _isAttached )
    {
        _sensorValid = std::make_shared< ItemRegister >( hal._colorSensorValid );
        _sensorRed = std::make_shared< ItemRegister >( hal._colorSensorRed );
        _sensorGreen = std::make_shared< ItemRegister >( hal._colorSensorGreen );
        _sensorBlue = std::make_shared< ItemRegister >( hal._colorSensorBlue );
        _sensorClear = std::make_shared< ItemRegister >( hal._colorSensorClear );

        while( _sensorValid->read< uint16_t >() != 0x01 )
        {
            QThread::msleep( 10 );
            qDebug() << "Wait sensor module to be ready...";
        }
    }

    qDebug() << "Sensor module is attached";

    _isAttached = true;

    return true;
}

void ColorSensor::changeTarget( Color colorTarget )
{
    _colorTarget = colorTarget;
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

    const uint16_t red = _sensorRed->read< uint16_t >();
    const uint16_t green = _sensorGreen->read< uint16_t >();
    const uint16_t blue = _sensorBlue->read< uint16_t >();
    const uint16_t clear = _sensorClear->read< uint16_t >();

    qDebug() << "Red:" << red << " Green:" << green << " Blue:" << blue << " Clear:" << clear;

    // TODO: read register, compare to threashold and check with target color
    return true;
}

Color ColorSensor::sensorCheck()
{
    const uint16_t red = _sensorRed->read< uint16_t >();
    const uint16_t green = _sensorGreen->read< uint16_t >();
    const uint16_t blue = _sensorBlue->read< uint16_t >();
    const uint16_t clear = _sensorClear->read< uint16_t >();

    qDebug() << "Red:" << red << " Green:" << green << " Blue:" << blue << " Clear:" << clear;

    // TODO: read register and compare to threshold
    return Color::Blue;
}

bool ColorSensor::isAttached() const
{
    return _isAttached;
}
