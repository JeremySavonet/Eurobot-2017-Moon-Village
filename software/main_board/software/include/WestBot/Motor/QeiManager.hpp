// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_QEIMANAGER_HPP_
#define WESTBOT_QEIMANAGER_HPP_

#include <stdint.h>

#include "../Memory/MemoryManager.hpp"

namespace WestBot {

class QeiManager
{
public:
    QeiManager( MemoryManager& memoryManager );
    ~QeiManager();

    int32_t getEncoderValue( int deviceId );

private:
    MemoryManager _memoryManager;
    uint32_t* _qeiMap;
};

}

#endif // WESTBOT_QEIMANAGER_HPP_
