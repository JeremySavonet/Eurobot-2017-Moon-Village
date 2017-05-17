// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_FUNNYACTION_HPP_
#define WESTBOT_FUNNYACTION_HPP_

#include <memory>

#include "Action.hpp"

namespace WestBot {

class FunnyAction : public Action
{
public:
    using Ptr = std::shared_ptr< FunnyAction >;

    /*!
    * \brief Constructor of FunnyAction.
    */
    FunnyAction();
    /*!
    * \brief Destructor.
    */
    ~FunnyAction() override = default;

    virtual void execute() override;
    virtual void skip() override;
};

}

#endif // WESTBOT_FUNNYACTION_HPP_
