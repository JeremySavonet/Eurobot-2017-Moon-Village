// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../include/WestBot/Motor/BrushlessMotorManager.hpp"

using namespace WestBot;

namespace
{
    const int BRUSHLESS_MOTOR_BASE_ADDRESS = 0x10008;
}

BrushlessMotorManager::BrushlessMotorManager( MemoryManager& memoryManager )
    : MotorManager( memoryManager, BRUSHLESS_MOTOR_BASE_ADDRESS )
{
}

BrushlessMotorManager::~BrushlessMotorManager()
{
}

void BrushlessMotorManager::disablePwm( int deviceId, int channel )
{

}

void BrushlessMotorManager::setPwm( int deviceId, int channel, int32_t value )
{

}

void BrushlessMotorManager::setPwm0( int deviceId, int32_t value )
{

}

void BrushlessMotorManager::setPwm1( int deviceId, int32_t value )
{

}

void BrushlessMotorManager::setPwmNegative0( int deviceId, int32_t value )
{

}

void BrushlessMotorManager::setPwmNegative1( int deviceId, int32_t value )
{

}
