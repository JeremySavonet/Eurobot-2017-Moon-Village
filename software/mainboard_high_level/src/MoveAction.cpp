// Copyright (c) 2017 All Rights Reserved WestBot

#include <WestBot/MoveAction.hpp>

using namespace WestBot;

MoveAction::MoveAction(
    TrajectoryManager& trajectoryManager,
    TrajectoryManager::TrajectoryType type,
    float theta,
    float distance,
    float x,
    float y,
    bool correction,
    bool doNotBlock )
    : Action()
    , _trajectoryManager( trajectoryManager )
    ,  _type( type )
    , _theta( theta )
    , _distance( distance )
    , _x( x )
    , _y( y )
    , _correction( correction )
    , _doNotBlock( doNotBlock )
{
}

void MoveAction::execute()
{
    switch( _type )
    {
    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_DISABLE:
        _trajectoryManager.disable();
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_ENABLE:
        _trajectoryManager.enable();
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_STOP:
        _trajectoryManager.stop();
        break;

   case TrajectoryManager::TrajectoryType::TYPE_TRAJ_HARDSTOP:
        _trajectoryManager.hardStop();
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_D_REL:
        _trajectoryManager.moveDRel( _distance, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL:
        _trajectoryManager.moveOnlyDRel( _distance, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_A_REL:
        _trajectoryManager.turnARel( _theta, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_A_ABS:
        _trajectoryManager.turnAAbs( _theta, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_A_REL:
        _trajectoryManager.turnOnlyARel( _theta, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_A_ABS:
        _trajectoryManager.turnOnlyAAbs( _theta, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_D_A_REL:
        _trajectoryManager.moveToDARel( _theta, _distance, _correction, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_TURNTO_XY:
        _trajectoryManager.turnToXY( _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_TURNTO_XY_BEHIND:
        _trajectoryManager.turnToXYBehind( _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_XY_ABS:
        _trajectoryManager.moveToXYAbs( _theta, _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS:
        _trajectoryManager.moveForwardToXYAbs( _theta, _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_BACKWARD_XY_ABS:
        _trajectoryManager.moveBackwardToXYAbs( _theta, _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_D_A_REL:
        // TODO: XXX
        break;

    case TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_XY_REL:
        _trajectoryManager.moveToXYRel( _x, _y, _doNotBlock );
        break;

    case TrajectoryManager::TrajectoryType::WAIT_TRAJ_READY:
        _trajectoryManager.waitTrajReady();
        break;
    }

    emit complete();
}
