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
    void turbineInsuffle();
    void turbineExpulse();
    void openArms90();
    void openArmsFull();
    void openArmsForFusee();
    void closeArms();
    void turnCarrousel();
    void ejectCylinder();
    void checkCylinderInCarrouselAtPosition( float position );
    void collectCylinderAtPosition( float theta, float x, float y );
    void collectTotemAtPosition( float theta, float x, float y );
    bool isCarrouselCanHandleTotems( int totemsNumber );

    // Wait
    void strategyWaitMs( int ms );

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
    void doStrat( const Color& color );
    void doFunnyAction();

private:
    SystemManager& _systemManager;
    TrajectoryManager& _trajectoryManager;
    Carrousel& _carrousel;
    Servo& _armRight;
    Servo& _armLeft;
    Servo& _ejector;
    Turbine& _turbine;

    bool _stratIsRunning;
};

}

#endif // WESTBOT_STRATEGYMANAGER_HPP_
