// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_HAL_HPP_
#define WESTBOT_HAL_HPP_

#include "MemoryManager.hpp"

namespace WestBot {

class Hal
{
public:
    Hal();
    ~Hal();

    void init();

private:
    MemoryManager _memoryManager;
};

}

#endif // WESTBOT_HAL_HPP_
