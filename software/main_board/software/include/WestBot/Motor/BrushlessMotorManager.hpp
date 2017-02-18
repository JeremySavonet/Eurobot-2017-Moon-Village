// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_BRUSHLESSMOTORMANAGER_HPP_
#define WESTBOT_BRUSHLESSMOTORMANAGER_HPP_

#include <stdint.h>

#include "MotorManager.hpp"

#include "../Memory/MemoryManager.hpp"

namespace WestBot {

class BrushlessMotorManager : public MotorManager
{
public:
    BrushlessMotorManager( MemoryManager& memoryManager );
    ~BrushlessMotorManager();

    void disablePwm( int deviceId, int channel ) override;
    void setPwm( int deviceId, int channel, int32_t value ) override;

    void setPwm0( int deviceId, int32_t value ) override;
    void setPwm1( int deviceId, int32_t value );

    void setPwmNegative0( int deviceId, int32_t value ) override;
    void setPwmNegative1( int deviceId, int32_t value ) override;
};

}

#endif // WESTBOT_BRUSHLESSMOTORMANAGER_HPP_
