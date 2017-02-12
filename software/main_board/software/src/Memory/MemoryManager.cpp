// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <iostream>
#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "hps_0.h"

#include "../../include/WestBot/Memory/MemoryManager.hpp"

using namespace WestBot;

namespace
{
    // The start address and length of the Lightweight bridge
    const int HPS_TO_FPGA_LW_BASE = 0xFF200000;
    const int HPS_TO_FPGA_LW_SPAN = 0x0020000;
}

MemoryManager::MemoryManager()
    : _lwBridgeMap( 0 )
    , _devMemFd( 0 )
{
    if( 0 != init() )
    {
        std::cout << "Failed to init MemoryManager module" << std::endl;
        return;
    }
}

MemoryManager::~MemoryManager()
{
    deInit();
}

// Public
uint32_t* MemoryManager::mapModuleToMemory( uint32_t moduleBaseAddress )
{
    // Set the moduleMap to the correct offset within the RAM
    // (moduleBaseAddress need to be in "hps_0.h")
     return ( uint32_t* )( _lwBridgeMap + moduleBaseAddress );
}

// Private
int MemoryManager::init()
{
    // Open up the /dev/mem device (aka, RAM)
    _devMemFd = open( "/dev/mem", O_RDWR | O_SYNC );
    if( _devMemFd < 0 )
    {
        std::cout << "Devmem open: failed" << std::endl;;
        return -1;
    }

    // mmap() the entire address space of the Lightweight bridge
    // so we can access our custom module
    _lwBridgeMap = ( uint32_t* )mmap( NULL,
                                      HPS_TO_FPGA_LW_SPAN,
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED,
                                      _devMemFd,
                                      HPS_TO_FPGA_LW_BASE );
    if( MAP_FAILED == _lwBridgeMap )
    {
        std::cout << "Devmem mmap: failed" << std::endl;;
        close( _devMemFd );
        return -1;
    }

    return 0;
}

void MemoryManager::deInit()
{
    // Unmap everything and close the /dev/mem file descriptor
    int result = munmap( _lwBridgeMap, HPS_TO_FPGA_LW_SPAN );
    if( result < 0 )
    {
        std::cout << "Devmem munmap: failed" << std::endl;;
        close( _devMemFd );
        return;
    }

    close( _devMemFd );
}
