// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_STRATEGYMANAGER_HPP_
#define WESTBOT_STRATEGYMANAGER_HPP_

#include <QList>
#include <QObject>
#include <QString>

#include "Action.hpp"
#include "Carrousel.hpp"
#include "ColorSensor.hpp"
#include "Common.hpp"
#include "Servo.hpp"
#include "TrajectoryManager.hpp"
#include "Turbine.hpp"

namespace WestBot
{
    class SystemManager;

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
        Servo& unblock,
        Turbine& turbine,
        ColorSensor& _colorSensor,
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
    Servo& _unblock;
    Turbine& _turbine;
    ColorSensor& _colorSensor;

    QList< Action::Ptr > _actions;
    Action::Ptr _currentAction;
    bool _stratIsRunning;
};

}

#endif // WESTBOT_STRATEGYMANAGER_HPP_
