// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

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
            qDebug() << "Rearming.";
            _trajectoryManager.enable();
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
    _armRight.write( SERVO_0_ARM_R_OPEN90 );
    _armLeft.write( SERVO_6_ARM_L_OPEN90 );
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
    _ejector.write( SERVO_7_EJECTOR_EJECT );
    QThread::msleep( 250 );
    _ejector.write( SERVO_7_EJECTOR_STANDBY );
}

void StrategyManager::checkCylinderInCarrouselAtPosition( float position )
{
    _carrousel.setPosition( position );
    QThread::msleep( 250 );

    // TODO: Check color
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
    return true;
}

void StrategyManager::strategyWaitMs( int ms )
{
    QThread::msleep( ms );
}

void StrategyManager::stopRobot()
{
    qDebug() << ">>>>>> OPPONENT DETECTED <<<<<<";

    if( _stratIsRunning )
    {
        _trajectoryManager.hardStop();
        strategyWaitMs( 200 );
    }
}

bool StrategyManager::gotoAvoidPosition(
    int xMm,
    int yMm,
    int aDeg,
    int trajEndFlags )
{
    qDebug() << "Enter go to avoid position";

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
void StrategyManager::doStrat( const Color& color )
{
    qDebug() << "Do strat for color:" << color;

    // Strat loop
    while( _stratIsRunning )
    {
        qDebug() << ">>>>> RUNNING <<<<<";

        if( color == Color::Yellow )
        {
            openArmsFull();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 430.0, -3.0 );

            closeArms();

            turnCarrouselCW();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 800.0, 0.0 );

            _trajectoryManager.turnAAbs( 180 );

            turnCarrouselCCW();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 430.0, 0.0 );

            openArms90();

            _trajectoryManager.moveBackwardToXYAbs( 0.0, 600.0, 0.0 );
        }
        else
        {
            openArmsFull();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 490.0, 40.0 );

            closeArms();

            turnCarrouselCW();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 840.0, 0.0 );

            _trajectoryManager.turnAAbs( -180 );

            turnCarrouselCCW();

            _trajectoryManager.moveForwardToXYAbs( 0.0, 470.0, 0.0 );

            openArms90();

            _trajectoryManager.moveBackwardToXYAbs( 0.0, 640.0, 0.0 );
        }

        _stratIsRunning = false;
    }
}

void StrategyManager::doFunnyAction()
{
    _trajectoryManager.hardStop();
}
