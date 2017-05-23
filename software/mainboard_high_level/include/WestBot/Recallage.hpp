// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_RECALLAGE_HPP_
#define WESTBOT_RECALLAGE_HPP_

#include "ItemRegister.hpp"

namespace WestBot
{

class Hal;

class Recallage
{
public:
    Recallage();

    void init( Hal& hal );

private:
    float* _theta;
    float* _distance;
    int _lenght;
    float _odoTheta;
    float _odoX;
    float _odoY;

    ItemRegister::Ptr _odoThetaReg;
    ItemRegister::Ptr _odoXReg;
    ItemRegister::Ptr _odoYReg;
};

}

#endif // WESTBOT_RECALLAGE_HPP_
