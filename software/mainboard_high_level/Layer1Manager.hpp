#ifndef WESTBOT_LAYER1MANAGER_HPP_
#define WESTBOT_LAYER1MANAGER_HPP_

#include <stdint.h>

#include <memory>

#include "Layer1.hpp"
#include "MemoryManager.hpp"

namespace WestBot {

class Layer1Manager
{
public:
    Layer1Manager( MemoryManager& manager, uint32_t baseAddress );

    ~Layer1Manager();

    void reset();

    uint32_t read( uint32_t offset, uint32_t size ) const;
    void write( uint32_t offset, uint32_t size, uint32_t data );

private:
    MemoryManager _memoryManager;
    Layer1* _layerMap;
};

}


#endif // WESTBOT_LAYER1MANAGER_HPP_
