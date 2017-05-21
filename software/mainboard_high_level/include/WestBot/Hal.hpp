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

    void clearRegisters();

private:
    MemoryManager _memoryManager;
    Memory _layer1;
    Memory _layer2;
    Memory _layer3;

public:
    // Layer 1   
    ItemRegister _resetAll; // Reset for layer 1 / layer 2 / layer 3
    ItemRegister _initOkL1;
    ItemRegister _modeSimu;
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

    ItemRegister _s0;
    ItemRegister _s0Enable;
    ItemRegister _s0Override;
    ItemRegister _s1;
    ItemRegister _s1Enable;
    ItemRegister _s1Override;
    ItemRegister _s2;
    ItemRegister _s2Enable;
    ItemRegister _s2Override;
    ItemRegister _s3;
    ItemRegister _s3Enable;
    ItemRegister _s3Override;
    ItemRegister _s4;
    ItemRegister _s4Enable;
    ItemRegister _s4Override;
    ItemRegister _s5;
    ItemRegister _s5Enable;
    ItemRegister _s5Override;
    ItemRegister _s6;
    ItemRegister _s6Enable;
    ItemRegister _s6Override;
    ItemRegister _s7;
    ItemRegister _s7Enable;
    ItemRegister _s7Override;

    ItemRegister _colorSensorValid;
    ItemRegister _colorSensorRed;
    ItemRegister _colorSensorGreen;
    ItemRegister _colorSensorBlue;
    ItemRegister _colorSensorClear;

    ItemRegister _distanceSensor;

    ItemRegister _esc0Enable;
    ItemRegister _esc0Override;
    ItemRegister _esc0Value;

    // Layer 2
    ItemRegister _odometryTheta;
    ItemRegister _odometryX;
    ItemRegister _odometryY;

    // PID DISTANCE
    ItemRegister _pidDistanceEnable;
    ItemRegister _pidDistanceOverride;
    ItemRegister _pidDistanceInverted;
    ItemRegister _pidDistanceKp;
    ItemRegister _pidDistanceKi;
    ItemRegister _pidDistanceKd;
    ItemRegister _pidDistanceSpeed;
    ItemRegister _pidDistanceAcceleration;
    ItemRegister _pidDistanceSaturation;
    ItemRegister _pidDistancePosition;
    ItemRegister _pidDistanceTarget;

    // PID ANGLE
    ItemRegister _pidAngleEnable;
    ItemRegister _pidAngleOverride;
    ItemRegister _pidAngleInverted;
    ItemRegister _pidAngleKp;
    ItemRegister _pidAngleKi;
    ItemRegister _pidAngleKd;
    ItemRegister _pidAngleSpeed;
    ItemRegister _pidAngleAcceleration;
    ItemRegister _pidAngleSaturation;
    ItemRegister _pidAnglePosition;
    ItemRegister _pidAngleTarget;

    // PID CARROUSEL
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

    // Layer 3
    ItemRegister _trajFreqHz;
    ItemRegister _trajCmdValid;
    ItemRegister _trajCmdId;
    ItemRegister _trajCmdType;
    ItemRegister _trajCmdOrderType;
    ItemRegister _trajCmdWndDistance;
    ItemRegister _trajCmdWndAngleDeg;
    ItemRegister _trajCmdWndAngleStartDeg;
    ItemRegister _trajCmdCfgSpeed;
    ItemRegister _trajCmdCfgAcc;
    ItemRegister _trajCmdPosTheta;
    ItemRegister _trajCmdPosX;
    ItemRegister _trajCmdPosY;
    ItemRegister _trajCmdDDistance;
    ItemRegister _trajOutAck;
    ItemRegister _trajOutState;
    ItemRegister _trajOutBlocked;
    ItemRegister _trajOutInWindow;
};

}

#endif // WESTBOT_HAL_HPP_
