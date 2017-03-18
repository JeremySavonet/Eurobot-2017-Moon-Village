// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MEMORY_HPP_
#define WESTBOT_MEMORY_HPP_

#include <stdint.h>

#include "Layer.hpp"
#include "MemoryManager.hpp"

namespace WestBot {

class Memory
{
public:
    Memory( MemoryManager& manager, uint32_t baseAddress );
    ~Memory();

    void reset();

    uint32_t read( uint32_t offset, uint32_t size ) const;
    void write( uint32_t offset, uint32_t size, uint32_t data );

private:
    MemoryManager _memoryManager;
    Layer_t* _layerMap;
};

}


#endif // WESTBOT_MEMORY_HPP_
