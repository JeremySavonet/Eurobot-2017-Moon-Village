// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_WAITACTION_HPP_
#define WESTBOT_WAITACTION_HPP_

#include <memory>

#include "Action.hpp"

namespace WestBot {

class WaitAction : public Action
{
public:
    using Ptr = std::shared_ptr< WaitAction >;

    WaitAction( int waitMs );

    void execute() override;

private:
    int _waitMs;
};

}

#endif // WESTBOT_WAITACTION_HPP_

