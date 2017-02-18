// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../include/WestBot/Motor/MotorManager.hpp"

using namespace WestBot;

MotorManager::MotorManager( MemoryManager& memoryManager,
                            uint32_t motorBaseAddress )
    : _memoryManager( memoryManager )
{
    // Init the memory for general motor
    _motorMap = _memoryManager.mapModuleToMemory( motorBaseAddress );
}

MotorManager::~MotorManager()
{
    _motorMap = 0;
}

void MotorManager::disablePwm( int deviceId, int channel )
{

}

void MotorManager::setPwm( int deviceId, int channel, int32_t value )
{

}

void MotorManager::setPwm0( int deviceId, int32_t value )
{

}

void MotorManager::setPwm1( int deviceId, int32_t value )
{

}

void MotorManager::setPwmNegative0( int deviceId, int32_t value )
{

}

void MotorManager::setPwmNegative1( int deviceId, int32_t value )
{

}

uint32_t* MotorManager::motorMap()
{
    return _motorMap;
}
