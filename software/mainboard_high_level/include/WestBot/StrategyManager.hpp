// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_STRATEGYMANAGER_HPP_
#define WESTBOT_STRATEGYMANAGER_HPP_

#include <QList>
#include <QObject>
#include <QString>

#include "Action.hpp"
#include "Carrousel.hpp"
#include "Common.hpp"
#include "Servo.hpp"
#include "SystemManager.hpp"
#include "TrajectoryManager.hpp"
#include "Turbine.hpp"

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
        Turbine& turbine,
        QObject* parent = nullptr );
    ~StrategyManager() override = default;

    // A set of actions
    void ejectCylinder();

    // Avoidance
    void stopRobot();
    bool gotoAvoidPosition( int xMm, int yMm, int aDeg, int trajEndFlags );
    bool gotoAvoidPositionRetry(
        int xMm,
        int yMm,
        int aDeg,
        int trajEndFlags,
        int numRetries );

private:
    void buildStrat( const Color& color );
    void doStrat( const Color& color );
    void doFunnyAction();
    void disableServos();
    void enableServos();

private:
    SystemManager& _systemManager;
    TrajectoryManager& _trajectoryManager;
    Carrousel& _carrousel;
    Servo& _armRight;
    Servo& _armLeft;
    Servo& _ejector;
    Turbine& _turbine;

    QList< Action::Ptr > _actions;

    bool _stratIsRunning;
};

}

#endif // WESTBOT_STRATEGYMANAGER_HPP_
