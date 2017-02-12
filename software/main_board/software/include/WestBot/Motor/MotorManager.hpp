// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MOTORMANAGER_HPP_
#define WESTBOT_MOTORMANAGER_HPP_

#include <stdint.h>

namespace WestBot {

class MotorManager
{
public:
    MotorManager();
    ~MotorManager();

    virtual void disablePwm( int deviceId, int channel );
    virtual void setPwm( int deviceId, int channel, int32_t value );

    virtual void setPwm0( int deviceId, int32_t value );
    virtual void setPwm1( int deviceId, int32_t value );

    virtual void setPwmNegative0( int deviceId, int32_t value );
    virtual void setPwmNegative1( int deviceId, int32_t value );
};

}

#endif // WESTBOT_MOTORMANAGER_HPP_
