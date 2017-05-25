// Copyright (c) 2017 All Rights Reserved WestBot

#include <WestBot/ConfigAction.hpp>

using namespace WestBot;

ConfigAction::ConfigAction(
    TrajectoryManager& trajectoryManager,
    TrajectoryManager::TrajectoryType type,
    float speed,
    float acc,
    float distance,
    float angle,
    float startAngle )
    : _trajectoryManager( trajectoryManager )
    , _type( type )
    , _speed( speed )
    , _acc( acc )
    , _distance( distance )
    , _angle( angle )
    , _startAngle( startAngle )

{
}

void ConfigAction::execute()
{
    switch( _type )
    {
    case  TrajectoryManager::TrajectoryType::DIST_CONFIG:
    {
        _trajectoryManager.setDistanceConfig( _speed, _acc );
    }
        break;

    case  TrajectoryManager::TrajectoryType::ANGLE_CONFIG:
    {
        _trajectoryManager.setAngleConfig( _speed, _acc );
    }
        break;

    case  TrajectoryManager::TrajectoryType::WINDOW_CONFIG:
    {
        _trajectoryManager.setWindow( _distance, _angle, _startAngle );
    }
        break;
    }
}
