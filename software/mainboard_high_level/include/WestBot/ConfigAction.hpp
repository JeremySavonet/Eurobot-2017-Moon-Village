// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_CONFIGACTION_HPP_
#define WESTBOT_CONFIGACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "TrajectoryManager.hpp"

namespace WestBot {

class ConfigAction : public Action
{
public:
    using Ptr = std::shared_ptr< ConfigAction >;

    ConfigAction(
        TrajectoryManager& trajectoryManager,
        TrajectoryManager::TrajectoryType type,
        float speed,
        float acc,
        float distance,
        float angle,
        float startAngle );

        void execute() override;

private:
    TrajectoryManager& _trajectoryManager;
    TrajectoryManager::TrajectoryType _type;
    float _speed;
    float _acc;
    float _distance;
    float _angle;
    float _startAngle;
};

}

#endif // WESTBOT_CONFIGACTION_HPP_

