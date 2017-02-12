// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_DCMOTORMANAGER_HPP_
#define WESTBOT_DCMOTORMANAGER_HPP_

#include <stdint.h>

#include "MotorManager.hpp"

#include "../Memory/MemoryManager.hpp"

namespace WestBot {

class DcMotorManager : public MotorManager
{
public:
    DcMotorManager( MemoryManager& memoryManager );
    ~DcMotorManager();

    void disablePwm( int deviceId, int channel ) override;
    void setPwm( int deviceId, int channel, int32_t value ) override;

    void setPwm0( int deviceId, int32_t value ) override;
    void setPwm1( int deviceId, int32_t value );

    void setPwmNegative0( int deviceId, int32_t value ) override;
    void setPwmNegative1( int deviceId, int32_t value ) override;
};

}

#endif // WESTBOT_DCMOTORMANAGER_HPP_
