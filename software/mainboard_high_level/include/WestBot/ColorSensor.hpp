// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_COLORSENSOR_HPP_
#define WESTBOT_COLORSENSOR_HPP_

#include <QString>

#include "Common.hpp"
#include "Hal.hpp"
#include "ItemRegister.hpp"

namespace WestBot {

class ColorSensor
{
public:
    ColorSensor( const QString& name );

    bool attach( Hal& hal,
                 Color colorTarget = Color::Unknown );

    void changeTarget( Color colorTarget );

    const QString& name() const;

    // Read the sensor and return a bool matching the targeted color.
    bool isInRange();

    // Read the sensor and return the guessed color.
    Color sensorCheck();

    bool isAttached() const;

private:
    const QString _name;

    ItemRegister::Ptr _sensorValid;
    ItemRegister::Ptr _sensorRed;
    ItemRegister::Ptr _sensorGreen;
    ItemRegister::Ptr _sensorBlue;
    ItemRegister::Ptr _sensorClear;

    Color _colorTarget;
    bool _isAttached;
};

}

#endif // WESTBOT_COLORSENSOR_HPP_
