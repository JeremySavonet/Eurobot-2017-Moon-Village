// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LED_HPP_
#define WESTBOT_LED_HPP_

#include <memory>

#include <stdint.h>

#include "MemoryManager.hpp"

namespace WestBot {

class Led
{
public:
    using Ptr = std::shared_ptr< Led >;

    Led( MemoryManager& manager ); // Default constructor
    ~Led(); // Destructor

    void turnOn( int ledId );
    void turnOff( int ledId );
    void reset();

private:
    MemoryManager _memoryManager;
    uint32_t *_ledMap;
};

}

#endif // WESTBOT_LED_HPP_
