// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_CARROUSEL_HPP_
#define WESTBOT_CARROUSEL_HPP_

#include "Hal.hpp"

namespace WestBot {

class Carrousel
{
public:
    enum class Sens
    {
        ClockWise,
        CounterClockWise
    };

    Carrousel( Hal& hal );

    bool init();

    float position();

    void setPosition( float position, bool closest = true );
    void setPosition( float position, Sens = Sens::ClockWise );

private:
    void setPeriod( int32_t period );
    void setEnable( bool enabled );
    void setOverride( bool enable );
    void setGainKp( float kp );
    void setGainKi( float ki );
    void setGainKd( float kd );
    void setSpeed( float speed );
    void setAcceleration( float acc );
    void setOutputSaturation( uint32_t sat );
    void setTarget( int32_t position );

    int32_t period() const;
    float kp() const;
    float ki() const;
    float kd() const;
    float speed() const;
    float acceleration() const;
    uint32_t outputSaturation() const;
    int32_t currentPosition() const;
    int32_t currentTarget() const;
    int32_t lastReference() const;

    int32_t reference() const;

private:
    Hal _hal;
    int32_t _reference;
    int32_t _nbTickPerTour;
};

}

#endif // WESTBOT_CARROUSEL_HPP_
