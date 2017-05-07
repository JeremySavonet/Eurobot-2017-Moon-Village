// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_TRAJECTORYMANAGER_HPP_
#define WESTBOT_TRAJECTORYMANAGER_HPP_

#include <QDebug>

#include "Hal.hpp"

#define CMD_TYPE_TRAJ 0x0
#define CMD_TYPE_CFG_DISTANCE 0x1
#define CMD_TYPE_CFG_ANGLE 0x2
#define CMD_TYPE_CFG_WND 0x3

#define TRAJ_DISABLE 0
#define TRAJ_ENABLE 1
#define TRAJ_STOP 2
#define TRAJ_HARDSTOP 3
#define TRAJ_D_REL 4
#define TRAJ_ONLY_D_REL 5
#define TRAJ_A_REL 6
#define TRAJ_A_ABS 7
#define TRAJ_ONLY_A_REL 8
#define TRAJ_ONLY_A_ABS 9
#define TRAJ_D_A_REL 10
#define TRAJ_TURNTO_XY 11
#define TRAJ_TURNTO_XY_BEHIND 12
#define TRAJ_GOTO_XY_ABS 13
#define TRAJ_GOTO_FORWARD_XY_ABS 14
#define TRAJ_GOTO_BACKWARD_XY_ABS 15
#define TRAJ_GOTO_D_A_REL 16
#define TRAJ_GOTO_XY_REL 17

namespace WestBot {

class TrajectoryManager
{
public:
    // State of the trajectory manager.
    enum class TrajectoryState
    {
        READY, // Ready, waiting for a trajectory.

        // Simple trajectories
        RUNNING_A, // Turning without moving
        RUNNING_D, // Going straight
        RUNNING_AD, // Going forward and turning at the same time

        // Trajectories using events
        RUNNING_XY_START,        // A trajectory order was given, preparing to go
        RUNNING_XY_ANGLE,        // Doing a preliminary turn before starting to move
        RUNNING_XY_ANGLE_OK,     // Angle is now ok, move forward
        RUNNING_XY_F_START,      // Same as RUNNING_XY_START but forward only
        RUNNING_XY_F_ANGLE,      // Same as RUNNING_XY_ANGLE but forward only
        RUNNING_XY_F_ANGLE_OK,   // Same as RUNNING_XY_ANGLE_OK but forward only
        RUNNING_XY_B_START,      // Same as RUNNING_XY_START but backward only
        RUNNING_XY_B_ANGLE,      // Same as RUNNING_XY_ANGLE but backward only
        RUNNING_XY_B_ANGLE_OK,   // Same as RUNNING_XY_B_ANGLE_OK but backward only

        // Circle
        RUNNING_CIRCLE,          // Running a circle trajectory

        // Line
        RUNNING_LINE,            // Following a line

        // Clitoid
        RUNNING_CLITOID_LINE,    // Running a clitoid (line->circle->line) in the line part
        RUNNING_CLITOID_CURVE,   // Running a clitoid in the curve part
    };

    /*!
    * \brief Overload stream operator for color enum class.
    */
    inline QDebug operator<<( QDebug debug, const TrajectoryState& state )
    {
        switch( state )
        {
        case TrajectoryState::READY:
            debug << "Trajectory: READY";
            break;

        case TrajectoryState::RUNNING_A:
            debug << "Trajectory: RUNNING_A";
            break;

        case TrajectoryState::RUNNING_D:
            debug << "Trajectory: RUNNING_D";
            break;

        case TrajectoryState::RUNNING_AD:
            debug << "Trajectory: RUNNING_AD";
            break;

        case TrajectoryState::RUNNING_XY_START:
            debug << "Trajectory: RUNNING_XY_START";
            break;

        case TrajectoryState::RUNNING_XY_ANGLE:
            debug << "Trajectory: RUNNING_XY_ANGLE";
            break;

        case TrajectoryState::RUNNING_XY_ANGLE_OK:
            debug << "Trajectory: RUNNING_XY_ANGLE_OK";
            break;

        case TrajectoryState::RUNNING_XY_F_START:
            debug << "Trajectory: RUNNING_XY_F_START";
            break;

        case TrajectoryState::RUNNING_XY_F_ANGLE:
            debug << "Trajectory: RUNNING_XY_F_ANGLE";
            break;

        case TrajectoryState::RUNNING_XY_F_ANGLE_OK:
            debug << "Trajectory: RUNNING_XY_F_ANGLE_OK";
            break;

        case TrajectoryState::RUNNING_XY_B_START:
            debug << "Trajectory: RUNNING_XY_B_START";
            break;

        case TrajectoryState::RUNNING_XY_B_ANGLE:
            debug << "Trajectory: RUNNING_XY_B_ANGLE";
            break;

        case TrajectoryState::RUNNING_XY_B_ANGLE_OK:
            debug << "Trajectory: RUNNING_XY_B_ANGLE_OK";
            break;

        case TrajectoryState::RUNNING_CIRCLE:
            debug << "Trajectory: RUNNING_CIRCLE";
            break;

        case TrajectoryState::RUNNING_LINE:
            debug << "Trajectory: RUNNING_LINE";
            break;

        case TrajectoryState::RUNNING_CLITOID_LINE:
            debug << "Trajectory: RUNNING_CLITOID_LINE";
            break;

        case TrajectoryState::RUNNING_CLITOID_CURVE:
            debug << "Trajectory: RUNNING_CLITOID_CURVE";
            break;
        }
        return debug;
    }

    TrajectoryManager( Hal& hal );
    ~TrajectoryManager();

    void init();

    void disable();
    void enable();

    void stop();
    void hardStop();

    void moveDRel( float distance );
    void moveOnlyDRel( float distance );
    void turnARel( float theta );
    void turnAAbs( float theta );
    void turnOnlyARel( float theta );
    void turnOnlyAAbs( float theta );
    void turnToXY( float x, float y );
    void turnToXYBehind( float x, float y );
    void moveToXYAbs( float theta, float x, float y );
    void moveForwardToXYAbs( float theta, float x, float y );
    void moveBackwardToXYAbs( float theta, float x, float y );
    void moveToDARel( float theta, float distance );
    void moveToXYRel( float x, float y );

private:
    Hal _hal;
};

}

#endif // WESTBOT_TRAJECTORYMANAGER_HPP_
