// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

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
    QObject* parent )
    : QObject( parent )
    , _systemManager( systemManager )
    , _trajectoryManager( trajectoryManager )
    , _carrousel( carrousel )
    , _armRight( armRight )
    , _armLeft( armLeft )
    , _ejector( ejector )
    , _stratIsRunning( false )
{
    connect(
        & _systemManager,
        & SystemManager::doStrat,
        this,
        [ this ]( const Color& color )
        {
            qDebug() << "Ready to start strategy thread...";
            doStrat( color );
        } );

    connect(
        & _systemManager,
        & SystemManager::doFunnyAction,
        this,
        [ this ]()
        {
            qDebug() << "Funny action time...";
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
            gotoAvoidPosition();
        } );
}

// Public methods
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

void StrategyManager::turnCarrousel()
{
    float pos = _carrousel.position();

    _carrousel.setPosition( pos - 1.0 );
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

    turnCarrousel();

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

    turnCarrousel();
    turnCarrousel();
    turnCarrousel();
    turnCarrousel();

    qDebug() << "Done collecting...";
}

void StrategyManager::gotoAvoidPosition()
{
    qDebug() << "Go to avoid position";

    _trajectoryManager.stop();
}

bool StrategyManager::isCarrouselCanHandleTotems( int totemsNumber )
{
    return true;
}


// Private methods
void StrategyManager::doStrat( const Color& color )
{
    qDebug() << "Do strat for color:" << color;

    // Strat loop
    while( _stratIsRunning )
    {
          collectTotemAtPosition( 10.0, 200.0, 200.0 );

    //    turnCarrousel();
    //    QThread::msleep( 1500 );
    //    turnCarrousel();
    //    QThread::msleep( 1500 );

        /*
        QThread::msleep( 250 );
        turnCarrousel();
        QThread::msleep( 250 );
        turnCarrousel();
        QThread::msleep( 250 );
        */

        /*
        _trajectoryManager.moveToXYAbs( 0.0, 200.0, 200.0 );

        _trajectoryManager.turnAAbs( 180.0 );

        _trajectoryManager.moveToXYAbs( 0.0, 200.0, 200.0 );
        */
    }
}

void StrategyManager::doFunnyAction()
{
    _trajectoryManager.hardStop();
}
