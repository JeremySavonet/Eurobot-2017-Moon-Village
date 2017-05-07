// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_STRATEGYMANAGER_HPP_
#define WESTBOT_STRATEGYMANAGER_HPP_

#include <QObject>
#include <QString>

#include "Carrousel.hpp"
#include "Common.hpp"
#include "Servo.hpp"
#include "SystemManager.hpp"
#include "TrajectoryManager.hpp"

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
        TrajectoryManager& trajectoryManager,
        Carrousel& carrousel,
        Servo& armRight,
        Servo& armLeft,
        Servo& ejector,
        QObject* parent = nullptr );
    ~StrategyManager() override = default;

private:
    void doStrat( const Color& color );
    void doFunnyAction();

private:
    SystemManager& _systemManager;
    TrajectoryManager& _trajectoryManager;
    Carrousel& _carrousel;
    Servo& _armRight;
    Servo& _armLeft;
    Servo& _ejector;
};

}

#endif // WESTBOT_STRATEGYMANAGER_HPP_
