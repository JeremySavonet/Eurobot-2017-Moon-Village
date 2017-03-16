// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_LAYER1_HPP_
#define WESTBOT_LAYER1_HPP_

namespace WestBot {

typedef struct Layer1
{
    uint32_t registers[ 64 ];

    /*
    uint32_t reset;
    uint32_t initOk;
    uint32_t voltage24V;
    uint32_t voltageA12V;
    uint32_t voltageA5V;
    uint32_t voltage12V;
    uint32_t buzzer;
    uint32_t inputValue;
    uint32_t inputOverride;
    uint32_t outputValue;
    uint32_t motorFaultReset;
    uint32_t m0Pwm;
    uint32_t m1Pwm;
    uint32_t m2Pwm;
    uint32_t m3Pwm;
    uint32_t m4Pwm;
    uint32_t m5Pwm;
    uint32_t m0Current;
    uint32_t m1Current;
    uint32_t m2Current;
    uint32_t m3Current;
    uint32_t m4Current;
    uint32_t m5Current;
    uint32_t qei0;
    uint32_t qei1;
    uint32_t qei2;
    uint32_t qei3;
    uint32_t servo1;
    uint32_t servo2;
    uint32_t servo3;
    uint32_t servo4;
    uint32_t servo5;
    uint32_t servo6;
    uint32_t servo7;
    uint32_t servo8;
    uint32_t an8CurrentS0;
    uint32_t an9CurrentS1;
    uint32_t an10CurrentS2;
    uint32_t an11CurrentS3;
    uint32_t an12CurrentS4;
    uint32_t an13CurrentS5;
    uint32_t an14CurrentS6;
    uint32_t an15CurrentS7;
    uint32_t esc0;
    uint32_t esc1;

    //Unused for now
    uint32_t unused1;
    uint32_t unused2;
    uint32_t unused3;
    uint32_t unused4;
    uint32_t unused5;
    uint32_t unused6;
    uint32_t unused7;
    uint32_t unused8;
    uint32_t unused9;
    uint32_t unused10;
    uint32_t unused11;
    uint32_t unused12;
    uint32_t unused13;
    uint32_t unused14;
    uint32_t unused15;
    uint32_t unused16;
    */
} __attribute__((packed)) Layer1_t;

}

#endif // WESTBOT_LAYER1_HPP_
