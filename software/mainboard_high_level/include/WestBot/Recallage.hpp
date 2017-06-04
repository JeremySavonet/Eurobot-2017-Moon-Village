// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_RECALLAGE_HPP_
#define WESTBOT_RECALLAGE_HPP_

#include "ItemRegister.hpp"

namespace WestBot
{

typedef struct {
    double x;
    double y;
    double theta;
} RobotPos;

class Hal;

class Recallage
{
public:
    Recallage();

    bool init( Hal& hal );

    void errorInit( double errX, double errY, double errTheta );

    void errorModify( double errX, double errY, double errTheta );

	bool calibrate(
        int len,
        const double* mesR,
        const double* mesTheta );

    RobotPos getPos(); // Unused for now
    RobotPos sendPos( const RobotPos& robotPos );

private:
    /*
    float* _theta;
    float* _distance;
    int _lenght;
    float _odoTheta;
    float _odoX;
    float _odoY;
    */
    ItemRegister::Ptr _odoThetaReg;
    ItemRegister::Ptr _odoXReg;
    ItemRegister::Ptr _odoYReg;
    bool _attached;

    RobotPos error = { 0, 0, 0 };

    struct {
        bool dir;
        double	ax;
        double	ay;
        double	bx;
        double	by;
    } tableBorder[6] = {
        { 0,      0, -1500,      0,      1500 },
        { 1,      0,  1500,   2000,      1500 },
        { 0,   2000,  1500,   2000,     -1500 },
        { 1,   2000, -1500,      0,     -1500 },
        { 0, 360+22, -1500, 360+22, -1500+710 },
        { 0, 360+22,  1500, 360+22,  1500-710 },
    };

    int tableBorderNb = sizeof( tableBorder ) / sizeof( tableBorder[ 0 ] );
};

}

#endif // WESTBOT_RECALLAGE_HPP_
