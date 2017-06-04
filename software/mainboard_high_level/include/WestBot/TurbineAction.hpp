// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_TURBINEACTION_HPP_
#define WESTBOT_TURBINEACTION_HPP_

#include <memory>

#include "Action.hpp"
#include "Turbine.hpp"

namespace WestBot {

class TurbineAction : public Action
{
public:
    using Ptr = std::shared_ptr< TurbineAction >;

    enum class Type
    {
        Insuffle = 0,
        Expulse
    };

    TurbineAction( Turbine& turbine, Type type );

    void execute() override;

private:
    Turbine& _turbine;
    Type _type;
};

}

#endif // WESTBOT_TURBINEACTION_HPP_

