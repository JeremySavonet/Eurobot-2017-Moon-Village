// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LED_HPP_
#define WESTBOT_LED_HPP_

#include <stdint.h>

#include <memory>

#include "MemoryManager.hpp"

namespace WestBot {

class Led
{
public:
    using Ptr = std::shared_ptr< Led >;

    Led( MemoryManager& manager );
    ~Led();

    void turnOn( int ledId );
    void turnOff( int ledId );
    void reset();

private:
    MemoryManager _memoryManager;
    uint32_t *_ledMap;
};

}

#endif // WESTBOT_LED_HPP_
