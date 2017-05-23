// Copyright (c) 2017 All Rights Reserved WestBot

// Author: E. Boiley
// Author: J. Savonet

#include <iostream>
#include <math.h>

#include <WestBot/Hal.hpp>
#include <WestBot/Recallage.hpp>

//#DEFINE TEST
#ifdef TEST

#include <QFile>

#endif

// 3rd party
#include  <3rdparty/Eigen/Dense>

using namespace Eigen;
using namespace WestBot;

namespace
{
    const double TELEM_THETA0 = -0.118;
    const double TELEM_X = 206.08;
    const double TELEM_Y = 101.1;
    const double TELEM_THETA = atan2(TELEM_Y,TELEM_X);
    const double TELEM_R = sqrt(TELEM_X*TELEM_X+TELEM_Y*TELEM_Y);

    const double ALGO_TELEM_DROP_MESURE = 100;
    const double ALGO_BORDER_ASSOCIATE_DROP = 150;
    const double ALGO_BORDER_FILTER_DROP = 7;
}

Recallage::Recallage()
    : _theta( nullptr )
    , _distance( nullptr )
    , _lenght( 0 )
    , _odoTheta( 0.0 )
    , _odoX( 0.0 )
    , _odoY( 0.0 )
    , _odoThetaReg( nullptr )
    , _odoXReg( nullptr )
    , _odoYReg( nullptr )
{

}

void Recallage::init( Hal& hal )
{

}
