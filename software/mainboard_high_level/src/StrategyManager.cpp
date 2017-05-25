// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>

#include <WestBot/ColorCheckerAction.hpp>
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
    Servo& pusher,
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
    , _pusher( pusher )
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

            _stratIsRunning = false;

            _actions.clear();

            // Stop all servos
            disableServos();

            // Stop carrousel
            _carrousel.enable( false );

            // Stop turbine
            _turbine.enable( false );
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
        QCoreApplication::processEvents();
    }

    return finished;
}

// Private methods
void StrategyManager::buildStrat( const Color& color )
{
    float inv = 1.0;
    float shift = 0.0;
    float offset = 0.0;

    if( color == Color::Yellow )
    {
        inv = 1.0;
        shift = 20.0;
    }
    else
    {
        inv = -1.0;
        shift = -18.0;
        offset = 90.0;
    }

    TurnCarrouselAction::Ptr turnCW =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CW );

    TurnCarrouselAction::Ptr turnCWMiddle =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CW_MID );

    TurnCarrouselAction::Ptr turnCCW =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CCW );

    TurnCarrouselAction::Ptr turnCCWMiddle =
        std::make_shared< TurnCarrouselAction >(
            _carrousel,
            TurnCarrouselAction::Sens::CCW_MID );

    WaitAction::Ptr wait500Ms =
        std::make_shared< WaitAction >( 500 );

    WaitAction::Ptr wait200Ms =
        std::make_shared< WaitAction >( 200 );

    MoveArmsAction::Ptr closeArms =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
           _unblock,
            _pusher,
            MoveArmsAction::Position::CLOSED );

    MoveArmsAction::Ptr eject =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
             _pusher,
            MoveArmsAction::Position::EJECT );

    MoveArmsAction::Ptr drop =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::DROP );

    MoveArmsAction::Ptr openArmsFull =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::OPEN_180 );

    MoveArmsAction::Ptr openArms45 =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::OPEN_45 );

    MoveArmsAction::Ptr openArms90 =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,       
            _pusher,
            MoveArmsAction::Position::OPEN_90 );

    MoveArmsAction::Ptr openArms0 =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::ZERO_POS );

    MoveArmsAction::Ptr pusherDeploy =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::PUSHER_DEPLOY );

    MoveArmsAction::Ptr pusherStandby =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::PUSHER_STANDBY );

    ColorCheckerAction::Ptr sensorAction =
        std::make_shared< ColorCheckerAction >(
            _colorSensor,
            ColorCheckerAction::Type::EJECT_GOOD );

    // >>>>>>>>>>>>> MOVE ACTIONS
    MoveAction::Ptr move1 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            600.0,
            500.0 * inv,
            false );

    MoveAction::Ptr move2 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            ( 1100.0 - 162.0 ),
            ( 1000.0 - 162.0 ) * inv,
            false );

    MoveAction::Ptr move3 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            ( 1100.0 + shift ),
            ( 900.0 + shift ) * inv,
            false );

    MoveAction::Ptr move4 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            (1400.0+shift) - 175.0,
            ( (600.0+shift) + 175.0 ) * inv,
            false );

    MoveAction::Ptr move7 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -80.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr move8 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            95.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr avance95SansCorrection =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            175.0,
            0.0,
            0.0,
            false );

    MoveAction::Ptr recul180AvecCorrection =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -180.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr move9 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            80.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr move10 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            100.0,
            0.0,
            0.0,
            false );

    MoveAction::Ptr move11 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -180.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr turnA45 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_A_ABS,
            -45.0 * inv,
            0.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr turn45 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_A_ABS,
            45.0 + offset,
            0.0,
            0.0,
            0.0,
            true );

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>><< OUR STRAT
    //_actions.push_back( openArms90 );
    _actions.push_back( move1 );
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms90 );
    _actions.push_back( move2 );
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms90 );
    _actions.push_back( move3 );
    _actions.push_back( move4 );
    _actions.push_back( turnA45 );
    _actions.push_back( openArms45 );
    _actions.push_back( wait200Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait200Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
    _actions.push_back( wait200Ms );
    _actions.push_back( move4 ); // Recallage sur position connue

    // On fait le deuxieme totem
    _actions.push_back( turnA45 );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait200Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
    _actions.push_back( wait200Ms );
    _actions.push_back( move4 ); // Recallage sur position connue

    // On fait le troisieme totem
    _actions.push_back( turnA45 );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait200Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
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
        //qDebug() << ">>>>>>>> CURRENT ACTION:" << _currentAction.get();
        //qDebug() << ">>>>>>>> REMAINING ACTIONS:" << _actions.size();

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
    _pusher.write( SERV0_DISABLE_CONSIGN );
}

void StrategyManager::enableServos()
{
    _unblock.write( SERVO_1_UNBLOCK_STANDBY );
    _ejector.write( SERVO_7_EJECTOR_STANDBY );
    _pusher.write( SERVO_5_PUSHER_STANDBY );

    QThread::msleep( 200 );
    _armLeft.write( SERVO_6_ARM_L_OPEN90 );
    _armRight.write( SERVO_0_ARM_R_OPEN90 );
}
