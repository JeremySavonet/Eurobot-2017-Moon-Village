// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_FUNNYACTION_HPP_
#define WESTBOT_FUNNYACTION_HPP_

#include <memory>

#include "Action.hpp"

class QProcess;

namespace WestBot {

class FunnyAction : public Action
{
public:
    using Ptr = std::shared_ptr< FunnyAction >;

    FunnyAction();

    void execute() override;

private:
    QProcess* _process;
};

}

#endif // WESTBOT_FUNNYACTION_HPP_

