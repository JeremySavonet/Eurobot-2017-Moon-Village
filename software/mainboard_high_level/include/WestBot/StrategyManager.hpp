// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_STRATEGYMANAGER_HPP_
#define WESTBOT_STRATEGYMANAGER_HPP_

#include <QObject>
#include <QString>

#include "SystemManager.hpp"

namespace WestBot
{

/*!
* \brief This class manage the robot strategy by handling data from FPGA
*        and push action in the action queue of the game manager.
*/
class StrategyManager : public QObject
{
public:
    StrategyManager(
        SystemManager& systemManager,
        QObject* parent = nullptr );
    ~StrategyManager() override = default;

private:
    SystemManager& _systemManager;
};

}

#endif // WESTBOT_STRATEGYMANAGER_HPP_
