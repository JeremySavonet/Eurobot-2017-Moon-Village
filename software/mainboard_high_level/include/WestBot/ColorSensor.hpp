// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_COLORSENSOR_HPP_
#define WESTBOT_COLORSENSOR_HPP_

#include <QString>

#include "Common.hpp"
#include "Hal.hpp"

namespace WestBot {

class ColorSensor
{
public:
    ColorSensor( const QString& name );

    bool attach( Hal& hal,
                 uint8_t sensorId,
                 Color colorTarget = Color::Unknown );

    const QString& name() const;

    // Read the sensor and return a bool matching the targeted color.
    bool isInRange();

    // Read the sensor and return the guessed color.
    Color sensorCheck();

    bool isAttached();

private:
    const QString _name;

    ItemRegister::Ptr _sensor;
    Color _colorTarget;
};

}

#endif // WESTBOT_COLORSENSOR_HPP_
