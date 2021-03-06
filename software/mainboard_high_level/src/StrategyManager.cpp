// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>

#include <WestBot/ColorCheckerAction.hpp>
#include <WestBot/ConfigAction.hpp>
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
 /*   connect(
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
		} );*/
}

void StrategyManager::stop()
{
    qDebug() << ">>>>>>>>>>>>>>>>>>>> FIN DU MATCH <<<<<<<<<<<<";
    _actions.clear();
    _stratIsRunning = false;

    // Stop traj
    _trajectoryManager.hardStop();
    _trajectoryManager.disable();

    disableServos();

    // Stop carrousel
    _carrousel.enable( false );
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
		//QCoreApplication::processEvents();
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

    WaitAction::Ptr wait100Ms =
        std::make_shared< WaitAction >( 100 );

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

    MoveArmsAction::Ptr openArmsFusee =
        std::make_shared< MoveArmsAction >(
            _armRight,
            _armLeft,
            _ejector,
            _unblock,
            _pusher,
            MoveArmsAction::Position::OPEN_FUSEE );

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
    MoveAction::Ptr waitTrajReady =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::WAIT_TRAJ_READY,
            0.0,
            0.0,
            0.0,
            0.0,
            false );

    MoveAction::Ptr moveTotem1 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            600.0,
            500.0 * inv,
			false );

    MoveAction::Ptr moveTotem2 =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            ( 1100.0 - 162.0 ),
            ( 1000.0 - 162.0 ) * inv,
            false,
			false );

    MoveAction::Ptr moveAlignementDepose =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            ( 1100.0 + shift ),
            ( 900.0 + shift ) * inv,
            false );

    MoveAction::Ptr moveDepose =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            (1400.0+shift) - 170.0,
            ( (600.0+shift) + 170.0 ) * inv,
            false );

    MoveAction::Ptr moveDeposePlus =
         std::make_shared< MoveAction >(
                _trajectoryManager,
                TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
                0.0,
                35.0,
                0.0,
                0.0,
                false );
        /*std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            (1400.0+shift) - 175.0 +30.0,
            ( (600.0+shift) + 175.0 -30.0) * inv,
            false );*/

    MoveAction::Ptr avance95SansCorrection =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            175.0,
            0.0,
            0.0,
            false );

    MoveAction::Ptr avance95SansCorrectionPlus =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            175.0 + 40.0,
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

    MoveAction::Ptr recul180AvecCorrectionPlus =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -180.0 - 40.0,
            0.0,
            0.0,
            true );

    MoveAction::Ptr recul50AvecCorrection =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -50.0,
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

    MoveAction::Ptr moveTotemUpper =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            1850.0 - 150.0,
            ( 700.0 + 50.0 ) * inv,
            true );

    MoveAction::Ptr moveBackBeforeEject =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_BACKWARD_XY_ABS,
            0.0,
            0.0,
            ( 1100.0 + shift ),
            ( 900.0 + shift ) * inv,
            false );

    MoveAction::Ptr turnToTotemBottom =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_TURNTO_XY,
            0.0,
            0.0,
            ( 790.0 ),
            ( 1500.0 - 365.0 ) * inv,
            false );

    MoveAction::Ptr moveToTotemBottom =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_GOTO_FORWARD_XY_ABS,
            0.0,
            0.0,
            ( 790.0 ),
            ( 1500.0 - 365.0 ) * inv,
            false );

    MoveAction::Ptr safeBackTotemBottom =
        std::make_shared< MoveAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::TYPE_TRAJ_ONLY_D_REL,
            0.0,
            -400.0,
            0.0,
            0.0,
            false );

    // CONFIG ACTION
   /* ConfigAction::Ptr normalSpeedAngle =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::ANGLE_CONFIG,
            0.0008,
            0.0000002,
            0.0,
            0.0,
            0.0 );

    ConfigAction::Ptr normalSpeedDistance =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::DIST_CONFIG,
            0.12,
            0.00001,
            0.0,
            0.0,
            0.0 );

    ConfigAction::Ptr fastSpeedAngle =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::ANGLE_CONFIG,
            0.0008,
            0.0000002,
            0.0,
            0.0,
            0.0 );

    ConfigAction::Ptr mediumSpeedDistance =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::DIST_CONFIG,
            0.10,
            0.00005,
            0.0,
            0.0,
            0.0 );

    ConfigAction::Ptr fastSpeedDistance =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::DIST_CONFIG,
            0.10,
            0.00006,
            0.0,
            0.0,
            0.0 );

    ConfigAction::Ptr windowPrecise =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::WINDOW_CONFIG,
            0.0,
            0.0,
            10.0,
            2.0,
            10.0 );

    ConfigAction::Ptr windowCourbe =
        std::make_shared< ConfigAction >(
            _trajectoryManager,
            TrajectoryManager::TrajectoryType::WINDOW_CONFIG,
            0.0,
            0.0,
            50.0,
            4.0,
            45.0 );*/

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>><< OUR STRAT
    //_actions.push_back( mediumSpeedDistance );
    //_actions.push_back( windowCourbe );
    _actions.push_back( openArms90 );
    _actions.push_back( wait200Ms );
    _actions.push_back( moveTotem1 );
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms90 );
    _actions.push_back( moveTotem2 );
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( drop );
    //actions.push_back( wait200Ms );
    _actions.push_back( turnCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms90 );
    //_actions.push_back( normalSpeedDistance );
    //_actions.push_back( windowPrecise );
    _actions.push_back( moveAlignementDepose );
    _actions.push_back( moveDepose );
    _actions.push_back( turnA45 );
    _actions.push_back( wait500Ms );

    _actions.push_back( openArms45 );
    _actions.push_back( wait500Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
// we redo the action to be safe
    _actions.push_back( openArms45 );
    _actions.push_back( wait500Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );

    _actions.push_back( sensorAction );
    _actions.push_back( eject );
    _actions.push_back( wait200Ms );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
    _actions.push_back( wait500Ms );
    _actions.push_back( moveDepose ); // Recallage sur position connue

    // On fait le deuxieme totem
    _actions.push_back( turnA45 );
    _actions.push_back( wait500Ms );

    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );
    _actions.push_back( wait200Ms );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
    _actions.push_back( wait200Ms );
    _actions.push_back( moveDepose ); // Recallage sur position connue

    // On fait le troisieme totem
    _actions.push_back( turnA45 );
    _actions.push_back( wait500Ms );

    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );
    _actions.push_back( wait200Ms );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( avance95SansCorrection );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );
    _actions.push_back( wait200Ms );

    // Totem upper
    _actions.push_back( openArms90 );
    //_actions.push_back( fastSpeedDistance );
    _actions.push_back( moveTotemUpper );
    _actions.push_back( closeArms );
    //_actions.push_back( windowCourbe );
    _actions.push_back( moveBackBeforeEject);
    //_actions.push_back( normalSpeedDistance );
    //_actions.push_back( windowPrecise );
    _actions.push_back( moveDepose ); // Recallage sur position connue
    _actions.push_back( turnA45 );
    _actions.push_back( wait500Ms );
    // we redo the action to be safe
        _actions.push_back( openArms45 );
        _actions.push_back( wait500Ms );
        _actions.push_back( closeArms );
        _actions.push_back( wait500Ms );
    _actions.push_back( eject );
    _actions.push_back( wait200Ms );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( avance95SansCorrectionPlus );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -80
    _actions.push_back( pusherStandby );

    // Totem bottom
    //_actions.push_back( fastSpeedDistance );
    _actions.push_back( turnToTotemBottom );
    _actions.push_back( openArms90 );
    _actions.push_back( moveToTotemBottom );
    _actions.push_back( openArms45 );
    _actions.push_back( closeArms );
    //_actions.push_back( windowCourbe );
    _actions.push_back( safeBackTotemBottom );
    //_actions.push_back( mediumSpeedDistance );
    //_actions.push_back( windowPrecise );
    _actions.push_back( moveDepose );
    //_actions.push_back( moveDeposePlus );
    _actions.push_back( turnA45 );
    _actions.push_back( wait500Ms );
    // we redo the action to be safe
        _actions.push_back( openArms45 );
        _actions.push_back( wait500Ms );
        _actions.push_back( closeArms );
        _actions.push_back( wait500Ms );
    _actions.push_back( eject );
    _actions.push_back( wait200Ms );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( avance95SansCorrectionPlus );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -80
    _actions.push_back( pusherStandby );

    /*
    _actions.push_back( eject );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait500Ms );
    _actions.push_back( openArmsFusee );
    _actions.push_back( wait100Ms );
    _actions.push_back( avance95SansCorrectionPlus );
    _actions.push_back( recul180AvecCorrectionPlus ); // Deplacement -80
    _actions.push_back( pusherStandby );
*/
    for( int i = 0; i < 100; i++ )
    {
        _actions.push_back( wait500Ms );
    }

    /*
    _actions.push_back( openArms90 );
    _actions.push_back( fastSpeedDistance );
    _actions.push_back( windowCourbe );
	_actions.push_back( moveTotem1 );
	_actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
	_actions.push_back( drop );
	_actions.push_back( wait100Ms );
	_actions.push_back( turnCW );
	_actions.push_back( wait200Ms );
	_actions.push_back( openArms90 );
    _actions.push_back( moveTotem2 ); // not blocking
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( drop );
    _actions.push_back( wait200Ms );
    _actions.push_back( turnCW );
    _actions.push_back( wait200Ms );
    _actions.push_back( openArms90 );
    _actions.push_back( mediumSpeedDistance );
    _actions.push_back( windowPrecise );
    _actions.push_back( moveAlignementDepose );
    _actions.push_back( moveDepose );
    _actions.push_back( turnA45 );
    _actions.push_back( openArms45 );
    _actions.push_back( wait100Ms );
    _actions.push_back( closeArms );
    _actions.push_back( wait500Ms );
    _actions.push_back( sensorAction );
    _actions.push_back( eject );

    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( wait200Ms );
    _actions.push_back( pusherDeploy );
    _actions.push_back( wait200Ms );
    _actions.push_back( moveDepose ); // Recallage sur position connue
    _actions.push_back( recul50AvecCorrection ); // Recallage sur position connue
    _actions.push_back( pusherStandby );
    _actions.push_back( moveDepose ); // Recallage sur position connue
    //_actions.push_back( wait200Ms );

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
    _actions.push_back( moveDepose ); // Recallage sur position connue
    _actions.push_back( recul50AvecCorrection ); // Recallage sur position connue
    _actions.push_back( pusherStandby );
    _actions.push_back( moveDepose ); // Recallage sur position connue
    //_actions.push_back( wait200Ms );

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

    // Dernier totem
    _actions.push_back( openArms90 );
    _actions.push_back( fastSpeedDistance );
    _actions.push_back( moveTotemUpper );
    _actions.push_back( closeArms );
    _actions.push_back( windowCourbe );
    _actions.push_back( moveBackBeforeEject);
    _actions.push_back( normalSpeedDistance );
    _actions.push_back( windowPrecise );
    _actions.push_back( moveDepose ); // Recallage sur position connue

    // On fait le deuxieme totem
    _actions.push_back( turnA45 );
    _actions.push_back( eject );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( pusherDeploy );
    _actions.push_back( openArmsFusee );
    _actions.push_back( wait100Ms );
    _actions.push_back( avance95SansCorrectionPlus );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );

    // Totem bottom
    _actions.push_back( fastSpeedDistance );
    _actions.push_back( turnToTotemBottom );
    _actions.push_back( openArms90 );
    _actions.push_back( moveToTotemBottom );
    _actions.push_back( openArms45 );
    _actions.push_back( closeArms );
    _actions.push_back( windowCourbe );
    _actions.push_back( safeBackTotemBottom );
    _actions.push_back( mediumSpeedDistance );
    _actions.push_back( windowPrecise );
    _actions.push_back( moveDepose );
    _actions.push_back( moveDeposePlus );
    _actions.push_back( turnA45 );
    _actions.push_back( eject );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -180
    _actions.push_back( openArms0 );
    _actions.push_back( pusherDeploy );
    _actions.push_back( openArmsFusee );
    _actions.push_back( wait100Ms );
    _actions.push_back( avance95SansCorrectionPlus );
    _actions.push_back( recul180AvecCorrection ); // Deplacement -80
    _actions.push_back( pusherStandby );

    */
}

void StrategyManager::doStrat( const Color& color )
{
	// Build the strat for selected color
	buildStrat( color );

	qDebug() << "Do strat for color:" << color;

	const QDateTime now;

	qDebug() << ">>>>>>>> ACTIONS SIZE" << _actions.size() << now.currentMSecsSinceEpoch();

	// Strat loop
	int i = 0;
	for( const auto& action: _actions )
	{
		_currentAction = action;
		action->execute();

		_actions.removeOne( action );
		qDebug() << now.currentDateTime().toMSecsSinceEpoch() << "Execute action number" << i;
		i++;

		/*if( ! _stratIsRunning )
		{
			qDebug() << QTime::currentTime().toString() << "Finish current action and stop after";
			break;
		}*/

		// Le big fail de jeremy !!!!
		//QCoreApplication::processEvents();
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
