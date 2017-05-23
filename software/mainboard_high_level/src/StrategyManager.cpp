// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include <WestBot/MoveAction.hpp>
#include <WestBot/MoveArmsAction.hpp>
#include <WestBot/TurbineAction.hpp>
#include <WestBot/TurnCarrouselAction.hpp>
#include <WestBot/WaitAction.hpp>
#include <WestBot/StrategyManager.hpp>
#include <WestBot/SystemManager.hpp>

using namespace WestBot;

StrategyManager::StrategyManager(
    SystemManager& systemManager,
    TrajectoryManager& trajectoryManager,
    Carrousel& carrousel,
    Servo& armRight,
    Servo& armLeft,
    Servo& ejector,
    Servo& unblock,
    Turbine& turbine,
    ColorSensor& colorSensor,
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
    , _trajectoryManager( trajectoryManager )
    , _carrousel( carrousel )
    , _armRight( armRight )
    , _armLeft( armLeft )
    , _ejector( ejector )
    , _unblock( unblock )
    , _turbine( turbine )
    , _colorSensor( colorSensor )
    , _currentAction( nullptr )
    , _stratIsRunning( false )
{
    connect(
        & _systemManager,
        & SystemManager::doStrat,
        this,
        [ this ]( const Color& color )
        {
            qDebug() << "Ready to start strategy thread...";
            _stratIsRunning = true;
            doStrat( color );
        } );

    connect(
        & _systemManager,
        & SystemManager::doFunnyAction,
        this,
        [ this ]()
        {
            qDebug() << "Funny action time...";
            _trajectoryManager.hardStop();
            _trajectoryManager.disable();
            _stratIsRunning = false;

            doFunnyAction();
        } );

    connect(
        & _systemManager,
        & SystemManager::hardStop,
        this,
        [ this ]()
        {
            qDebug() << "Hard stop requested.";

            // Stop traj
            _trajectoryManager.hardStop();
            _trajectoryManager.disable();

            // Stop all servos
            disableServos();

            // Stop carrousel
            _carrousel.enable( false );

            // Stop turbine
            _turbine.enable( false );

            _stratIsRunning = false;
        } );

    connect(
        & _systemManager,
        & SystemManager::reArming,
        this,
        [ this ]()
        {
            qDebug() << "Rearming.";

            // Enable traj
            _trajectoryManager.enable();

            // Init arms position
            enableServos();

            // Enable carrousel
            _carrousel.enable( true );

            // Enable turbine
            _turbine.enable( true );
        } );

    connect(
        & _systemManager,
        & SystemManager::opponentDetected,
        this,
        [ this ]()
        {
            stopRobot();
        } );
}

// Public methods
void StrategyManager::stopRobot()
{
    if( _stratIsRunning )
    {
        _trajectoryManager.hardStop();
        _trajectoryManager.disable();

        MoveAction::Ptr safety =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
                0.0,
                -300.0,
                0.0,
                0.0,
                false );

        _trajectoryManager.enable();

        // This will insert the safety action to be executed first
        // and then back to the last action
        _actions.push_front( safety );
        _actions.push_front( _currentAction );
    }
}

bool StrategyManager::gotoAvoidPosition(
    int xMm,
    int yMm,
    int aDeg,
    int trajEndFlags )
{
    Q_UNUSED( trajEndFlags );

    qDebug() << "Enter go to avoid position";
    qDebug()
        << "Want to move at X:" << xMm << " Y:" << yMm << " A:" << aDeg;

    // TODO:
    // Create a point at opponent position
    // Create a point a current position
    // Find path to avoid position point
    // Build point vector
    // Iterate through this vector and send forward xy abs command

    return true;
}

bool StrategyManager::gotoAvoidPositionRetry(
    int xMm,
    int yMm,
    int aDeg,
    int trajEndFlags,
    int numRetries )
{
    qDebug() << "Enter go to avoid position retry";

    bool finished = false;
    int counter = 0;

    while( ! finished )
    {
        qDebug() << "Try attempt number" << counter;
        finished = gotoAvoidPosition( xMm, yMm, aDeg, trajEndFlags );
        counter++;

        // Exit when maximum number of retries is reached
        // Negative number of retries means infinite number of retries
        if( numRetries >= 0 && counter > numRetries )
        {
            break;
        }
    }

    return finished;
}

// Private methods
void StrategyManager::buildStrat( const Color& color )
{
    TurnCarrouselAction::Ptr turnCW =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CW );

    TurnCarrouselAction::Ptr turnCCW =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CCW );

    WaitAction::Ptr wait1s =
        std::make_shared< WaitAction >( 1000 );

    WaitAction::Ptr wait500Ms =
        std::make_shared< WaitAction >( 500 );

    WaitAction::Ptr wait200Ms =
        std::make_shared< WaitAction >( 200 );

    MoveArmsAction::Ptr disableArms =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            MoveArmsAction::Position::DISABLE );

    MoveArmsAction::Ptr closeArms =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
           _unblock,
            MoveArmsAction::Position::CLOSED );

    MoveArmsAction::Ptr eject =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            MoveArmsAction::Position::EJECT );

    MoveArmsAction::Ptr drop =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            MoveArmsAction::Position::DROP );

    MoveArmsAction::Ptr openArmsFull =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            MoveArmsAction::Position::OPEN_180 );

    if( color == Color::Yellow )
    {
        MoveAction::Ptr move1 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                430.0,
                -3.0,
                false );

        MoveAction::Ptr move2 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                400.0,
                -565.0,
                false );

        MoveAction::Ptr move3 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                895.0,
                -565.0,
                false );

        MoveAction::Ptr move4 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                910.0,
                -565.0,
                false );

        MoveAction::Ptr move5 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                800.0,
                -565.0,
                false );

        MoveAction::Ptr move6 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                935.0,
                -565.0,
                false );

        MoveAction::Ptr move7 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                840.0,
                -565.0,
                false );

        // HOMOLOGATION SEQUENCE
        _actions.clear();
        _actions.push_back( turnCCW );
        _actions.push_back( move1 );
        _actions.push_back( closeArms );
        _actions.push_back( turnCW );
        _actions.push_back( move2 );
        _actions.push_back( move3 );
        _actions.push_back( turnCCW );
        _actions.push_back( wait1s );
        _actions.push_back( eject );

        /*_actions.push_back( wait1s );
        _actions.push_back( move4 );
        _actions.push_back( move5 );
        _actions.push_back( drop );

        _actions.push_back( wait500Ms );
        _actions.push_back( disableArms );
        */
        /*
        _actions.push_back( move6 );
        _actions.push_back( move7 );
        _actions.push_back( turnCW );
        _actions.push_back( wait500Ms );

        // Eject cylindre 2
        _actions.push_back( drop );
        _actions.push_back( wait200Ms );
        */
        /*
        // Eject cylinre 2 =============//

        _armRight.write( SERVO_0_ARM_R_DROP );
        _armLeft.write( SERVO_6_ARM_L_DROP );

        strategyWaitMs( 200 );

        _ejector.write( SERVO_7_EJECTOR_EJECT );

        strategyWaitMs( 75 );

        _armRight.write( SERVO_0_ARM_R_OPEN40 );
        _armLeft.write( SERVO_6_ARM_L_OPEN40 );
        strategyWaitMs( 1000 );

        _ejector.write( SERVO_7_EJECTOR_STANDBY );
        strategyWaitMs( 500 );

        disableServos();

        // ======================= //

        _trajectoryManager.moveBackwardToXYAbs( 0.0, 808.0, -565.0 );

        _armRight.write( SERVO_0_ARM_R_OPEN40 );
        _armLeft.write( SERVO_6_ARM_L_OPEN40 );
        strategyWaitMs( 500 );
        disableServos();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 930.0, -565.0 );
        _trajectoryManager.moveBackwardToXYAbs( 0.0, 800.0, -565.0 );

        _trajectoryManager.moveBackwardToXYAbs( 0.0, 900.0, -565.0 );

        _trajectoryManager.moveBackwardToXYAbs( 0.0, 1140.0, -565.0 );

        _trajectoryManager.moveForwardToXYAbs( 0.0, 600.0, -565.0 );
        _trajectoryManager.moveForwardToXYAbs( 0.0, 600.0, 0.0 );
        _trajectoryManager.moveBackwardToXYAbs( 0.0, 50.0, 0.0 );
        */
    }
    else
    {
        MoveAction::Ptr move1 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                850.0,
                180.0,
                false );

        MoveAction::Ptr move2 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                1100.0,
                -400.0,
                false );

        MoveAction::Ptr move3 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                750.0,
                -620.0,
                false );

        MoveAction::Ptr move4 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_BACKWARD_XY_ABS,
                0.0,
                0.0,
                1100.0,
                -350.0,
                false );

        MoveAction::Ptr move5 =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
                0.0,
                0.0,
                1150.0,
                -285.0,
                false );

        // HOMOLOGATION SEQUENCE BLUE
        _actions.clear();
        _actions.push_back( turnCCW );
        _actions.push_back( wait1s );
        _actions.push_back( move1 );
        _actions.push_back( closeArms );
        _actions.push_back( turnCW );
        _actions.push_back( wait1s );
        _actions.push_back( openArmsFull );
        _actions.push_back( move2 );
        _actions.push_back( closeArms );
        _actions.push_back( turnCW );
        _actions.push_back( wait1s );
        _actions.push_back( openArmsFull );
        _actions.push_back( move3 );
        _actions.push_back( closeArms );
        _actions.push_back( turnCW );
        _actions.push_back( wait1s );
        _actions.push_back( move4 );
        _actions.push_back( move5 );
    }
}

void StrategyManager::doStrat( const Color& color )
{
    // Build the strat for selected color
    buildStrat( color );

    qDebug() << "Do strat for color:" << color;
/*
    MoveArmsAction::Ptr closeArms =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
           _unblock,
            MoveArmsAction::Position::CLOSED );

    closeArms->execute();

    _colorSensor.sensorCheck();
*/
    // Strat loop
    for( const auto& action: _actions )
    {
        _currentAction = action;
        action->execute();
        _actions.removeOne( action );

        if( ! _stratIsRunning )
        {
            qDebug() << "Finish current action and stop after";
            break;
        }
    }

    _actions.clear();
}

void StrategyManager::doFunnyAction()
{
    // Stop and disable robot
    _trajectoryManager.hardStop();
    _trajectoryManager.disable();

    // Execute funny action
    connect(
        & _funnyAction,
        & Action::complete,
        this,
        [ this ]()
        {
            qDebug() << "Action complete";
            _funnyAction.deleteLater();
        } );

    _funnyAction.execute();
}

// Helpers to be safe
void StrategyManager::disableServos()
{
    _armLeft.write( SERV0_DISABLE_CONSIGN );
    _armRight.write( SERV0_DISABLE_CONSIGN );
    _ejector.write( SERV0_DISABLE_CONSIGN );
    _unblock.write( SERV0_DISABLE_CONSIGN );
}

void StrategyManager::enableServos()
{
    _unblock.write( SERVO_1_UNBLOCK_STANDBY );
    _ejector.write( SERVO_7_EJECTOR_STANDBY );
    QThread::msleep( 200 );
    _armLeft.write( SERVO_6_ARM_L_OPEN90 );
    _armRight.write( SERVO_0_ARM_R_OPEN90 );
}
