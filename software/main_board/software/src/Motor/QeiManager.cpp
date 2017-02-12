// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../include/WestBot/Motor/QeiManager.hpp"

using namespace WestBot;

namespace
{
    const int QEI_BASE_ADDRESS = 0x10008;
}

QeiManager::QeiManager( MemoryManager& memoryManager )
    : _memoryManager( memoryManager )
    , _qeiMap( 0 )
{
    // Init the memory for QEI
    _qeiMap = _memoryManager.mapModuleToMemory( QEI_BASE_ADDRESS );
}

QeiManager::~QeiManager()
{
    _qeiMap = 0;
}

int32_t QeiManager::getEncoderValue( int deviceId )
{
    *_qeiMap = 0xFF;
    return *_qeiMap; // TODO: XXX
}
