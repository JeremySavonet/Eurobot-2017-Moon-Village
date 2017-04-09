// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MOVEACTION_HPP_
#define WESTBOT_MOVEACTION_HPP_

#include <memory>

#include "Action.hpp"

namespace WestBot {

class MoveAction : public Action
{
public:
    using Ptr = std::shared_ptr< MoveAction >;

    enum class Type
    {
        TRAJ_STOP = 0,
        TRAJ_HARDSTOP,
        TRAJ_D_REL,
        TRAJ_ONLY_D_REL,
        TRAJ_A_REL,
        TRAJ_A_ABS,
        TRAJ_ONLY_A_REL,
        TRAJ_ONLY_A_ABS,
        TRAJ_D_A_REL,
        TRAJ_TURNTO_XY,
        TRAJ_TURNTO_XY_BEHIND,
        TRAJ_GOTO_XY_ABS,
        TRAJ_GOTO_FORWARD_XY_ABS,
        TRAJ_GOTO_BACKWARD_XY_ABS,
        TRAJ_GOTO_D_A_REL,
        TRAJ_GOTO_XY_REL
    };

    MoveAction( Type type );
    ~MoveAction() override = default;

    void execute() override;
    void skip() override;

private:
    Type _type;
};

}

#endif // WESTBOT_MOVEACTION_HPP_
