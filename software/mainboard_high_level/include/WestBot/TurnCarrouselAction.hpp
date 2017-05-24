// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_TURNCARROUSELACTION_HPP_
#define WESTBOT_TURNCARROUSELACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "Carrousel.hpp"

namespace WestBot {

class TurnCarrouselAction : public Action
{
public:
    using Ptr = std::shared_ptr< TurnCarrouselAction >;

    enum class Sens
    {
        CW = 0,
        CCW,
        CW_MID,
        CCW_MID
    };

    TurnCarrouselAction(
        Carrousel& carrousel,
        Sens sens );

    void execute() override;

private:
    Carrousel& _carrousel;
    Sens _sens;
};

}

#endif // WESTBOT_TURNCARROUSELACTION_HPP_

