// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MOTORMANAGER_HPP_
#define WESTBOT_MOTORMANAGER_HPP_

#include <stdint.h>

#include "../Memory/MemoryManager.hpp"

namespace WestBot {

class MotorManager
{
public:
    MotorManager( MemoryManager& memoryManager, uint32_t motorBaseAddress );
    ~MotorManager();

    virtual void disablePwm( int deviceId, int channel );
    virtual void setPwm( int deviceId, int channel, int32_t value );

    virtual void setPwm0( int deviceId, int32_t value );
    virtual void setPwm1( int deviceId, int32_t value );

    virtual void setPwmNegative0( int deviceId, int32_t value );
    virtual void setPwmNegative1( int deviceId, int32_t value );

protected:
    uint32_t* motorMap();

private:
    MemoryManager _memoryManager;
    uint32_t* _motorMap;
};

}

#endif // WESTBOT_MOTORMANAGER_HPP_
