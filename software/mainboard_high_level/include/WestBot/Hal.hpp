// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_HAL_HPP_
#define WESTBOT_HAL_HPP_

#include "ItemRegister.hpp"
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
    Memory _layer1;
    Memory _layer2;
    Memory _layer3;

public:
    ItemRegister _resetL1;
    ItemRegister _congigL1;
    ItemRegister _stateL1;
    ItemRegister _voltage24V;
    ItemRegister _voltageA12V;
    ItemRegister _voltageA5V;
    ItemRegister _voltage12V;
    ItemRegister _buzzer;
    ItemRegister _buzzerOverride;
    ItemRegister _input0;
    ItemRegister _input1;
    ItemRegister _input2;
    ItemRegister _input3;
    ItemRegister _inputOverride;
    ItemRegister _output0;
    ItemRegister _output1;
    ItemRegister _output2;
    ItemRegister _output3;
    ItemRegister _outputOverride;
    // TODO: complete layer1 registers defition

    ItemRegister _carrouselPidPeriod;
    ItemRegister _carrouselEnable;
    ItemRegister _carrouselOverride;
    ItemRegister _carrouselPidKp;
    ItemRegister _carrouselPidKi;
    ItemRegister _carrouselPidKd;
    ItemRegister _carrouselSpeed;
    ItemRegister _carrouselAcc;
    ItemRegister _carrouselOutputSaturation;
    ItemRegister _carrouselPosition;
    ItemRegister _carrouselTarget;
    ItemRegister _carrouselLastReference;
};

}

#endif // WESTBOT_HAL_HPP_
