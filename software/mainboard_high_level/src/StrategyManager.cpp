// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>

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
    , _color( Color::Unknown )
{
    connect(
        & _systemManager,
        & SystemManager::doStrat,
        this,
        [ this ]( const Color& color )
        {
            qDebug()
                << QTime::currentTime().toString() << "Ready to start strategy thread...";
            _color = color;
            _stratIsRunning = true;
            doStrat( color );
        } );

    connect(
        & _systemManager,
        & SystemManager::doFunnyAction,
        this,
        [ this ]()
        {
            qDebug()
                << QTime::currentTime().toString() << "Funny action time...";
            doFunnyAction();
        } );

    connect(
        & _systemManager,
        & SystemManager::hardStop,
        this,
        [ this ]()
        {
            qDebug()
                << QTime::currentTime().toString() << "Hard stop requested.";

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
        & SystemManager::stopped,
        this,
        [ this ]()
        {
            // Stop traj
            _trajectoryManager.hardStop();
            _trajectoryManager.disable();
            _stratIsRunning = false;
        } );

    connect(
        & _systemManager,
        & SystemManager::reArming,
        this,
        [ this ]()
        {
            qDebug() << QTime::currentTime().toString() << "Rearming.";

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
        _stratIsRunning = false;
        _trajectoryManager.hardStop();
        _trajectoryManager.disable();

        disableServos();

        // Stop carrousel
        _carrousel.enable( false );

        // Stop turbine
        _turbine.enable( false );

        qDebug() << ">>>>>>>> LAST ACTION STOPPED:" << _currentAction.get();
        qDebug() << ">>>>>>>>>>>> REMAINING ACTIONS:" << _actions.size();

       /* MoveAction::Ptr safety =
            std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
                0.0,
                -300.0,
                0.0,
                0.0,
                false );

        _trajectoryManager.enable();

        qDebug() << ">>>>>>> BEFORE INSERTING NEW ACTION";

        // This will insert the safety action to be executed first
        // and then back to the last action
        _actions.push_front( safety );
        _actions.push_front( _currentAction );

        qDebug() << ">>>>>>> AFTER INSERTING NEW ACTION" << _actions.size();
        _stratIsRunning = true;
        doStrat( _color );
        */
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
    // Strat loop
    if( color == Color::Yellow )
    {
        // TODO: XXX
    }
    else
    {
        // TODO: XXX
    }
}

void StrategyManager::doStrat( const Color& color )
{
    // Build the strat for selected color
    buildStrat( color );

    qDebug() << "Do strat for color:" << color;

    qDebug() << ">>>>>>>> ACTIONS SIZE" << _actions.size();

    // Strat loop
    for( const auto& action: _actions )
    {
        _currentAction = action;
        action->execute();
        _actions.removeOne( action );
        qDebug() << QTime::currentTime().toString() << "Execute action";
        qDebug() << ">>>>>>>> CURRENT ACTION:" << _currentAction.get();
        qDebug() << ">>>>>>>> REMAINING ACTIONS:" << _actions.size();

        if( ! _stratIsRunning )
        {
            qDebug() << QTime::currentTime().toString() << "Finish current action and stop after";
            break;
        }
    }

    qDebug()
        << QTime::currentTime().toString()
        << "Strat is over. Make sure we have clear the action list";

    _stratIsRunning = false;
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
            qDebug() << QTime::currentTime().toString() << "Action complete";
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
