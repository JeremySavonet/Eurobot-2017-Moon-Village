// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include <WestBot/MoveAction.hpp>
#include <WestBot/StrategyManager.hpp>

using namespace WestBot;

StrategyManager::StrategyManager(
    SystemManager& systemManager,
    TrajectoryManager& trajectoryManager,
    Carrousel& carrousel,
    Servo& armRight,
    Servo& armLeft,
    Servo& ejector,
    Turbine& turbine,
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
    , _trajectoryManager( trajectoryManager )
    , _carrousel( carrousel )
    , _armRight( armRight )
    , _armLeft( armLeft )
    , _ejector( ejector )
    , _turbine( turbine )
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
            openArms90();

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
void StrategyManager::disableServos()
{
    _armLeft.write( SERV0_DISABLE_CONSIGN );
    _armRight.write( SERV0_DISABLE_CONSIGN );
    _ejector.write( SERV0_DISABLE_CONSIGN );
}

void StrategyManager::turbineInsuffle()
{
    _turbine.setValue( TURBINE_INSUFFLE_VALUE );
}

void StrategyManager::turbineExpulse()
{
    _turbine.setValue( TURBINE_EXPULSE_VALUE );
}

void StrategyManager::openArms90()
{
    _armRight.write( SERVO_0_ARM_R_OPEN60 );
    _armLeft.write( SERVO_6_ARM_L_OPEN60 );
}

void StrategyManager::openArmsFull()
{
    _armRight.write( SERVO_0_ARM_R_OPEN );
    _armLeft.write( SERVO_6_ARM_L_OPEN );
}

void StrategyManager::openArmsForFusee()
{
    _ejector.write( SERVO_7_EJECTOR_FUSEE );
    QThread::msleep( 250 );

    openArmsFull();
    QThread::msleep( 250 );

    _armLeft.write( SERV0_DISABLE_CONSIGN );
    _armRight.write( SERV0_DISABLE_CONSIGN );
}

void StrategyManager::closeArms()
{
    _armRight.write( SERVO_0_ARM_R_CLOSED );
    _armLeft.write( SERVO_6_ARM_L_CLOSED );
}

void StrategyManager::turnCarrouselCW()
{
    float pos = _carrousel.position();

    _carrousel.setPosition( pos - 1.0 );
}

void StrategyManager::turnCarrouselCCW()
{
    float pos = _carrousel.position();

    _carrousel.setPosition( pos + 1.0 );
}

void StrategyManager::ejectCylinder()
{
    _armRight.write( SERVO_0_ARM_R_DROP );
    _armLeft.write( SERVO_6_ARM_L_DROP );

    strategyWaitMs( 200 );

    _ejector.write( SERVO_7_EJECTOR_EJECT );

    strategyWaitMs( 250 );

    openArms90();
    strategyWaitMs( 500 );

    _ejector.write( SERVO_7_EJECTOR_STANDBY );
    strategyWaitMs( 500 );

    disableServos();
}

void StrategyManager::checkCylinderInCarrouselAtPosition( float position )
{
    _carrousel.setPosition( position );
    QThread::msleep( 250 );
}

void StrategyManager::collectCylinderAtPosition( float theta, float x, float y )
{
    openArmsFull();

    _trajectoryManager.moveToXYAbs( theta, x, y );

    turnCarrouselCW();

    qDebug() << "Done collecting...";
}

void StrategyManager::collectTotemAtPosition( float theta, float x, float y )
{
    if( ! isCarrouselCanHandleTotems( 4 ) )
    {
        qDebug() << "Carrousel is not empty and can only handle"
                 << 2 << "totems";
        return;
    }

    openArmsForFusee();

    _trajectoryManager.moveToXYAbs( theta, x, y );

    turnCarrouselCW();
    turnCarrouselCW();
    turnCarrouselCW();
    turnCarrouselCW();

    qDebug() << "Done collecting...";
}

bool StrategyManager::isCarrouselCanHandleTotems( int totemsNumber )
{
    Q_UNUSED( totemsNumber );
    return true;
}

void StrategyManager::strategyWaitMs( int ms )
{
    QThread::msleep( ms );
}

void StrategyManager::stopRobot()
{
    if( _stratIsRunning )
    {
        _trajectoryManager.hardStop();
        _trajectoryManager.disable();
        strategyWaitMs( 2000 );
        qDebug() << "Changing trajectory...";
        _trajectoryManager.enable();
        _trajectoryManager.moveOnlyDRel( -100.0, false );
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
    if( color == Color::Yellow )
    {
        MoveAction::Ptr move1 = std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            430.0,
            -3.0 );

        _actions.push_back( move1 );

        /*
        turnCarrouselCCW();

        strategyWaitMs( 1000 );

        _trajectoryManager.moveForwardToXYAbs( 0.0, 430.0, -3.0 );

        closeArms();

        turnCarrouselCW();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 400.0, -565.0 );

        _trajectoryManager.moveForwardToXYAbs( 0.0, 895.0, -565.0 );

        turnCarrouselCCW();

        ejectCylinder();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 910.0, -565.0 );
        _trajectoryManager.moveBackwardToXYAbs( 0.0, 800.0, -565.0 );

        _armRight.write( SERVO_0_ARM_R_DROP );
        _armLeft.write( SERVO_6_ARM_L_DROP );
        strategyWaitMs( 500 );
        disableServos();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 935.0, -565.0 );
        _trajectoryManager.moveBackwardToXYAbs( 0.0, 840.0, -565.0 );

        turnCarrouselCW();

        strategyWaitMs( 500 );

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
        /*
        turnCarrouselCCW();

        strategyWaitMs( 1000 );

        _trajectoryManager.moveForwardToXYAbs( 0.0, 850.0, 180.0 );

        closeArms();

        turnCarrouselCW();

        strategyWaitMs( 1000 );

        openArmsFull();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 1110.0, -400.0 );

        closeArms();

        turnCarrouselCW();

        strategyWaitMs( 1000 );

        openArmsFull();

        _trajectoryManager.moveForwardToXYAbs( 0.0, 750.0, -620.0 );

        closeArms();

        turnCarrouselCW();

        strategyWaitMs( 1000 );

        _trajectoryManager.moveBackwardToXYAbs( 0.0, 1100.0, -350.0 );
        _trajectoryManager.moveForwardToXYAbs( 0.0, 1150.0, -285.0 );
        */
    }
}

void StrategyManager::doStrat( const Color& color )
{
    qDebug() << "Do strat for color:" << color;

    buildStrat( color );

    // Strat loop
    while( _stratIsRunning )
    {

        for( const auto& action: _actions )
        {
            action->execute();
        }

        _stratIsRunning = false;
    }
}

void StrategyManager::doFunnyAction()
{
    _trajectoryManager.hardStop();
}
