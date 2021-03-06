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

    bool attach( Hal& hal );

    void changeTarget( Color colorTarget );

    const QString& name() const;

    // Read the sensor and return the guessed color.
    int sensorCheck();

    bool isAttached() const;

    bool checkIsEmpty();

private:
    const QString _name;

    ItemRegister::Ptr _motorOverride;
    ItemRegister::Ptr _motorInverted;
    ItemRegister::Ptr _motorValue;
    ItemRegister::Ptr _sensorValid;
    ItemRegister::Ptr _sensorRed;
    ItemRegister::Ptr _sensorGreen;
    ItemRegister::Ptr _sensorBlue;
    ItemRegister::Ptr _sensorClear;

    uint32_t _vcAvg;

    Color _colorTarget;
    bool _isAttached;
};

}

#endif // WESTBOT_COLORSENSOR_HPP_
