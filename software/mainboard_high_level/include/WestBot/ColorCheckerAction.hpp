// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_COLORCHECKERACTION_HPP_
#define WESTBOT_COLORCHECKERACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "ColorSensor.hpp"

namespace WestBot {

class ColorCheckerAction : public Action
{
public:
    using Ptr = std::shared_ptr< ColorCheckerAction >;

    enum class Type
    {
        CHECK_EMPTY = 0,
        EJECT_WRONG,
        EJECT_GOOD
    };

    ColorCheckerAction(
        ColorSensor& colorSensor,
        Type type );

    void execute() override;

private:
    ColorSensor& _colorSensor;
    Type _type;
};

}

#endif // WESTBOT_COLORCHECKERACTION_HPP_

