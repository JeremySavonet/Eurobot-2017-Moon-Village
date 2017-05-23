// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_MOVEARMSACTION_HPP_
#define WESTBOT_MOVEARMSACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "Servo.hpp"

namespace WestBot {

class MoveArmsAction : public Action
{
public:
    using Ptr = std::shared_ptr< MoveArmsAction >;

    enum class Position
    {
        CLOSED = 0,
        ZERO_POS,
        DROP,
        OPEN_45,
        OPEN_90,
        OPEN_180,
        OPEN_FUSEE,
        DISABLE,
        EJECT,
        UNBLOCK,
        STANDBY_UNBLOCK
    };

    MoveArmsAction(
        Servo& armRight,
        Servo& armLeft,
        Servo& ejector,
        Servo& unblock,
        Position position );

    void execute() override;

private:
    Servo& _armRight;
    Servo& _armLeft;
    Servo& _ejector;
    Servo& _unblock;
    Position _position;
};

}

#endif // WESTBOT_MOVEARMSACTION_HPP_

