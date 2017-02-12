// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../include/WestBot/Motor/DcMotorManager.hpp"

using namespace WestBot;

namespace
{
    const int DC_MOTOR_BASE_ADDRESS = 0x10008;
}

DcMotorManager::DcMotorManager( MemoryManager& memoryManager )
    : MotorManager( memoryManager, DC_MOTOR_BASE_ADDRESS )
{
}

DcMotorManager::~DcMotorManager()
{
}

void DcMotorManager::disablePwm( int deviceId, int channel )
{

}

void DcMotorManager::setPwm( int deviceId, int channel, int32_t value )
{

}

void DcMotorManager::setPwm0( int deviceId, int32_t value )
{

}

void DcMotorManager::setPwm1( int deviceId, int32_t value )
{

}

void DcMotorManager::setPwmNegative0( int deviceId, int32_t value )
{

}

void DcMotorManager::setPwmNegative1( int deviceId, int32_t value )
{

}
