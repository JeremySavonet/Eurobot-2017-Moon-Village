// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_PID_HPP_
#define WESTBOT_PID_HPP_

#include <stdint.h>

namespace WestBot {

class Pid
{
public:
    Pid();
    ~Pid();

    void reset();

    void setGains( int16_t gainP, int16_t gainI, int16_t gainD );

    void setMaximums( int32_t maxIn, int32_t maxI, int32_t maxOut );

    void setOutShift( uint8_t outShift );

    int8_t setDerivateFilter( uint8_t nbSamples );

    // Getters
    int16_t gainP() const;
    int16_t gainI() const;
    int16_t gainD() const;
    int32_t maxIn() const;
    int32_t maxI() const;
    int32_t maxOut() const;
    uint8_t outShift() const;
    uint8_t derivateFilter() const;

    int32_t getValueI() const;

    int32_t getValueIn() const;

    int32_t getValueD() const;

    int32_t getValueOut() const;

    // PID process
    int32_t compute( int32_t in );

private:
    void init();

private:
    int16_t _gainP;
    int16_t _gainI;
    int16_t _gainD;

    uint8_t _outShift;

    uint8_t _derivateNbSamples;

    uint8_t _index;

    int32_t _prevSamples[ 4 ];

    int32_t _maxIn;
    int32_t _maxI;
    int32_t _maxOut;

    int32_t _integral;
    int32_t _prevD;
    int32_t _prevOut;
};

}

#endif // WESTBOT_PID_HPP_
