// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "../../../include/WestBot/ControlSystem/Filters/Pid.hpp"

using namespace WestBot;

namespace
{
    void Saturate( int32_t toSaturate, int32_t valueMax )
    {
        if( toSaturate > valueMax )
        {
            toSaturate = valueMax;
        }
        else if( toSaturate < -valueMax )
        {
            toSaturate = -valueMax;
        }
    }
}

Pid::Pid()
    : _gainP( 1 )
    , _gainI( 0 )
    , _gainD( 0 )
    , _outShift( 0 )
    , _derivateNbSamples( 1 )
    , _index( 0 )
    , _prevSamples{ 0, 0, 0, 0 }
    , _maxIn( 0 )
    , _maxI( 0 )
    , _maxOut( 0 )
    , _integral( 0 )
    , _prevD( 0 )
    , _prevOut( 0 )
{
}

Pid::~Pid()
{
    reset();
}

void Pid::reset()
{
    _gainP = 0;
    _gainI = 0;
    _gainD = 0;
    _outShift = 0;
    _derivateNbSamples = 0;
    _index = 0;
    _prevSamples[ 4 ] = 0;
    _maxIn = 0;
    _maxI = 0;
    _maxOut = 0;
    _integral = 0;
    _prevD = 0;
    _prevOut = 0;

}

void Pid::setGains( int16_t gainP, int16_t gainI, int16_t gainD )
{
    _gainP = gainP;
    _gainI = gainI;
    _gainD = gainD;
}

void Pid::setMaximums( int32_t maxIn, int32_t maxI, int32_t maxOut )
{
    _maxIn = maxIn;
    _maxI = maxI;
    _maxOut = _maxOut;
}

void Pid::setOutShift( uint8_t outShift )
{
    _outShift = outShift;
}

int8_t Pid::setDerivateFilter( uint8_t nbSamples )
{
    int8_t ret;
    if( nbSamples > 4 )
    {
        ret = -1;
    }
    else
    {
        _derivateNbSamples = nbSamples;
        ret = 0;
    }

    return ret;
}

// Getters
int16_t Pid::gainP() const
{
    return _gainP;
}

int16_t Pid::gainI() const
{
    return _gainI;
}

int16_t Pid::gainD() const
{
    return _gainD;
}

int32_t Pid::maxIn() const
{
    return _maxIn;
}

int32_t Pid::maxI() const
{
    return _maxI;
}

int32_t Pid::maxOut() const
{
    return _maxOut;
}

uint8_t Pid::outShift() const
{
    return _outShift;
}

uint8_t Pid::derivateFilter() const
{
    return _derivateNbSamples;
}

int32_t Pid::getValueI() const
{
    return _integral;
}

int32_t Pid::getValueIn() const
{
    return _prevSamples[ _index ];
}

int32_t Pid::getValueD() const
{
    return _prevD;
}

int32_t Pid::getValueOut() const
{
    return _prevOut;
}

// PID process
int32_t Pid::compute( int32_t in )
{
    int32_t derivate;
    int32_t command;
    uint8_t prevIndex;

    /*!
     * \brief Integral value : the integral become bigger with time .. (think
     *        to area of graph, we add one area to the previous) so,
     *        integral = previous integral + current value
     */

    /* derivate value
     *             f(t+h) - f(t)        with f(t+h) = current value
     *  derivate = -------------             f(t)   = previous value
     *                    h
     * so derivate = current error - previous error
     *
     * We can apply a filter to reduce noise on the derivate term,
     * by using a bigger period.
     */

    prevIndex = _index + 1;
    if( prevIndex >= _derivateNbSamples )
    {
        prevIndex = 0;
    }

    // saturate input... it influences integral an derivate
    if( _maxIn )
    {
        ::Saturate( in, _maxIn) ;
    }

    derivate = in - _prevSamples[ prevIndex ];
    _integral += in;

    if( _maxI )
    {
        ::Saturate( _integral, _maxI );
    }

    // so, command = P.coef_P + I.coef_I + D.coef_D
    command = in * _gainP +
              _integral * _gainI +
              ( derivate * _gainD ) / _derivateNbSamples;

    if( command < 0 )
    {
        command = -( -command >> _outShift );
    }
    else
    {
        command = command >> _outShift ;
    }

    if( _maxOut )
    {
        ::Saturate( command, _maxOut );
    }

    // Backup of current error value (for the next calcul of derivate value)
    _prevSamples[ _index ] = in ;
    _index = prevIndex; // Next index is prev_index
    _prevD = derivate ;
    _prevOut = command ;

    return command;
}
