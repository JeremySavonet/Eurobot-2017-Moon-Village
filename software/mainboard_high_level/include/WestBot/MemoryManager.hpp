// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MEMORYMANAGER_HPP_
#define WESTBOT_MEMORYMANAGER_HPP_

#include <stdint.h>

namespace WestBot {

class MemoryManager
{
public:
    MemoryManager();
    ~MemoryManager();

    uint32_t* mapModuleToMemory( uint32_t moduleBaseAddress );

private:
    int init();
    void deInit();

private:
    void* _lwBridgeMap;
    int _devMemFd;
};

}

#endif // WESTBOT_MEMORYMANAGER_HPP__
