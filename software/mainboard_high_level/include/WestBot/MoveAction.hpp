// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_MOVEACTION_HPP_
#define WESTBOT_MOVEACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "TrajectoryManager.hpp"

namespace WestBot {

class MoveAction : public Action
{
public:
    using Ptr = std::shared_ptr< MoveAction >;

    static MoveAction moveThetaCorrection( float theta, bool correction );
    static MoveAction moveDistanceCorrection( float distance, bool correction );
    static MoveAction moveThetaDistanceCorrection(
        float theta,
        float distance,
        bool correction );

    static MoveAction moveXY( float x, float y );
    static MoveAction move thetaXY( float theta, float x, float y );

    ~MoveAction() override = default;

    void execute() override;

private:
    MoveAction(
        TrajectoryManager& trajectoryManager,
        TrajectoryManager::TrajectoryType type,
        float theta,
        float distance,
        float x,
        float y,
        bool correction );

    TrajectoryManager& _trajectoryManager;
    TrajectoryManager::TrajectoryType _type;
    float _theta;
    float _distance;
    float _x;
    float _y;
    bool _correction;
};

}

#endif // WESTBOT_MOVEACTION_HPP_
