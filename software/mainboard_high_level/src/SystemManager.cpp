// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QFile>
#include <QState>
#include <QString>
#include <QThread>
#include <QTime>

#include <Defines.hpp>
#include <WestBot/SystemManager.hpp>

using namespace WestBot;

namespace
{
    const int FUNNY_ACTION = 80 * 1000; // 90s

    const int GAME_DURATION = 90 * 1000; // 90s
}

SystemManager::SystemManager( Hal& hal, QObject* parent )
    : QObject( parent )
    , _hal( hal )
    , _stateMachine( this )
    , _startButton( new Input( std::make_shared< ItemRegister >( _hal._input0 ),
                               "Tirette" ) )
    , _colorButton( new Input( std::make_shared< ItemRegister >( _hal._input1 ),
                               "Color" ) )
    , _stopButton( new Input( std::make_shared< ItemRegister >( _hal._input2 ),
                              "AU" ) )
    , _ledYellow( new Output( std::make_shared< ItemRegister >( _hal._output0 ),
                              "yellow" ) )
    , _ledBlue( new Output( std::make_shared< ItemRegister >( _hal._output2 ),
                            "blue" ) )
    , _color( Color::Unknown )
    , _colorSensor( "Color_sensor" )
    , _armRight( "Arm_right" )
    , _armLeft( "Arm_left" )
    , _ejector( "Ejector" )
    , _unblock( "Unblock" )
    , _turbine( "Turbine" )
    , _carrousel( _hal )
    , _trajectoryManager( _hal )
    , _systemMode( SystemManager::SystemMode::Full )
    , _lidar( "/dev/ttyUSB0" )
    , _detectionManager( "Opponent_detector" )
    , _strategyManager( *this,
                        _trajectoryManager,
                        _carrousel,
                        _armRight,
                        _armLeft,
                        _ejector,
                        _unblock,
                        _turbine,
                        _colorSensor )
{
    connect(
        & _funnyTimer,
        & QTimer::timeout,
        this,
        & SystemManager::doFunnyAction );

    // Task to notify that the robot is alive
    connect(
        & _aliveTimer,
        & QTimer::timeout,
        this,
        & SystemManager::robotAlive );

    connect(
        _startButton.get(),
        & Input::stateChanged,
        this,
        [ this ]( const DigitalValue& value )
        {
            if( value == DigitalValue::OFF )
            {
                if( _stopButton->digitalRead() == DigitalValue::ON )
                {
                    qWarning()
                        << QTime::currentTime().toString() << "Could not start game if AU is on";
                }
                else
                {
                    emit started();
                }
            }
        } );

    connect(
        _colorButton.get(),
        & Input::stateChanged,
        this,
        [ this ]( const DigitalValue& value )
        {
            displayColor( value );
            _colorSensor.changeTarget( _color );
        } );

    connect(
        _stopButton.get(),
        & Input::stateChanged,
        this,
        [ this ]( const DigitalValue& value )
        {
            if( value == DigitalValue::ON )
            {
                emit hardStop();
                _aliveTimer.stop();
            }
            else
            {
                emit reArming();
            }
        } );

    connect(
        & _detectionManager,
        & DetectionManager::opponentDetected,
        this,
        [ this ]( bool status )
        {
            if( status )
            {
                qDebug()
                    << QTime::currentTime().toString() << "Opponent detected";
                emit opponentDetected();
            }
            else
            {
                qDebug() << QTime::currentTime().toString() << "We are safe";
            }
        } );
}

SystemManager::~SystemManager()
{
    _lidar.disconnect();

    stop();
}

// Public methods
bool SystemManager::init()
{
    // Always reset the system at startup
    reset();

    // Config PID Distance
    _hal._pidDistanceEnable.write( 0 );
    _hal._pidDistanceOverride.write( 0 );
    _hal._pidDistanceInverted.write( 0 );
    _hal._pidDistanceKp.write( ( float ) 2000.0 );
    _hal._pidDistanceKi.write( ( float ) 0.0 );
    _hal._pidDistanceKd.write( ( float ) 0.0 );

    _hal._pidDistanceSpeed.write( ( float ) 0.01 );
    _hal._pidDistanceAcceleration.write( ( float ) 0.0001 );
    _hal._pidDistanceSaturation.write( 25000 );

    _hal._pidDistanceTarget.write( _hal._pidDistancePosition.read< float >() );
    _hal._pidDistanceEnable.write( 1 );

    // Config PID Angle
    _hal._pidAngleEnable.write( 0 );
    _hal._pidAngleOverride.write( 0 );
    _hal._pidAngleInverted.write( 1 );
    _hal._pidAngleKp.write( ( float ) 500000.0 );
    _hal._pidAngleKi.write( ( float ) 0.0 );
    _hal._pidAngleKd.write( ( float ) 0.0 );

    _hal._pidAngleSpeed.write( ( float ) 0.0001 );
    _hal._pidAngleAcceleration.write( ( float ) 0.00000002 );
    _hal._pidAngleSaturation.write( 25000 );

    _hal._pidAngleTarget.write( _hal._pidAnglePosition.read< float >() );
    _hal._pidAngleEnable.write( 1 );

    _armRight.attach( _hal, 0, SERVO_0_ARM_R_OPEN90, SERVO_0_ARM_R_CLOSED );

    if( ! _armRight.isAttached() )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach servo arm right...";
        return false;
    }

    _unblock.attach( _hal, 1, SERVO_1_UNBLOCK_STANDBY, SERVO_1_UNBLOCK_ACTION );

    if( ! _unblock.isAttached() )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach servo unblock...";
        return false;
    }

    _armLeft.attach( _hal, 6, SERVO_6_ARM_L_OPEN90, SERVO_6_ARM_L_CLOSED );

    if( ! _armLeft.isAttached() )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach servo arm left...";
        return false;
    }

    _ejector.attach( _hal, 7, SERVO_7_EJECTOR_STANDBY, SERVO_7_EJECTOR_EJECT );

    if( ! _ejector.isAttached() )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach servo ejector...";
        return false;
    }

    // Color sensor
    if( ! _colorSensor.attach( _hal ) )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach color sensor module";
        return false;
    }

    // Distance sensor
    if( ! _detectionManager.init( _hal ) )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to init proximity sensor module";
        return false;
    }

    if( ! _recallage.init( _hal ) )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to init recallage module";
        return false;
    }

    if( ! _lidar.connect() )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to connect to RPLidar";
        return false;
    }

    if( ! _turbine.attach( _hal ) )
    {
        qWarning()
            << QTime::currentTime().toString() << "Failed to attach turbine...";
        return false;
    }

    if( ! _colorSensor.checkIsEmpty() )
    {
        qDebug()
            << QTime::currentTime().toString() << "Failed to finish init: Carrousel is not empty.";
        return false;
    }

    if( ! _carrousel.init() )
    {
        qWarning() << QTime::currentTime().toString() << "Failed to init carrousel module...";
        return false;
    }

    // Set carrousel offset
    _carrousel.setPosition( 1.05 );

    _trajectoryManager.init();

    // Override output registers
    _hal._outputOverride.write( 0x01010101 );

    displayColor( _colorButton->digitalRead() );

    /*
    // Start lidar scan
    if( ! _lidar.startScan() )
    {
        qWarning() << "Error: cannot start the scan operation.";
        return false;
    }

    // TODO: flush and grab data
    _lidar.flushScanData();

    int len = 0;
    double distance[ MAX_LEN_SCAN_DATA ];
    double angle[ MAX_LEN_SCAN_DATA ];

    if( ! _lidar.grabScanData( distance, angle, & len ) )
    {
        qDebug() << "Error: Unable to grab data";
        return false;
    }

    QFile scanData( "scanData.raw" );

    if( ! scanData.open( QIODevice::ReadWrite ) )
    {
        qDebug() << "Failed to open file for data storage";
        return false;
    }

    RobotPos currentPos = _recallage.calibrate( len, distance, angle );
    qDebug() << ">>>>>>>> Current pos" << currentPos.theta << currentPos.x << currentPos.y;
    */
    _gameTimer.setSingleShot( true );
    _funnyTimer.setSingleShot( true );

    createStateMachine();

    return true;
}

void SystemManager::start()
{
    _stateMachine.start();
}

void SystemManager::stop()
{
    _stateMachine.stop();
}

void SystemManager::reset()
{
    _hal._colorEnable.write( 0 );

    QThread::msleep( 200 );

    _hal._resetAll.write( 1 );

    _hal.clearRegisters();

    _hal._resetAll.write( 0 );

    _hal._colorEnable.write( 1 );

    qDebug() << QTime::currentTime().toString() << "System was reset";
}

void SystemManager::setMode( SystemManager::SystemMode mode )
{
    _systemMode = mode;
}

SystemManager::SystemMode SystemManager::mode() const
{
    return _systemMode;
}

const Color& SystemManager::color() const
{
    return _color;
}

// Private methods
void SystemManager::createStateMachine()
{
    QState* initialState = createInitialState( & _stateMachine );
    QState* checkGameColorState = createCheckGameColorState( & _stateMachine );
    QState* startGameState = createStartGameState( & _stateMachine );

    QState* runningStratState = createRunningStratState( & _stateMachine );

    QState* stopGameState = createStopGameState( & _stateMachine );

    QState* errorState = createErrorState( & _stateMachine );

    QState* hardStopState = createHardStopState( & _stateMachine );

    // Add state transitions
    initialState->addTransition( checkGameColorState );

    checkGameColorState->addTransition(
        this,
        & SystemManager::started,
        startGameState );

    startGameState->addTransition(
        this,
        & SystemManager::readyForWar,
        runningStratState );

    runningStratState->addTransition(
        & _gameTimer,
        & QTimer::timeout,
        stopGameState );

    // Rearm the system for an other game
    stopGameState->addTransition( initialState );

    // Fallback error state
    checkGameColorState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    startGameState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    runningStratState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    stopGameState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    // Hard stop fallback
    checkGameColorState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    startGameState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    runningStratState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    stopGameState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    hardStopState->addTransition(
        this,
        & SystemManager::reArming,
        initialState );

    _stateMachine.setInitialState( initialState );
}

// States before the game start (init, check color, tirette)
QState* SystemManager::createInitialState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        []()
        {
            qDebug() << QTime::currentTime().toString() << "Enter initial state";
        } );

    connect(
        state,
        & QState::exited,
        this,
        [ this ]()
        {
            emit systemInitialized();
        } );

    return state;
}

QState* SystemManager::createCheckGameColorState( QState* parent )
{
    // This state need to be blocking. Polling on color
    // button IO. Exit the state when startEvent occured.
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        []()
        {
            qDebug() << QTime::currentTime().toString() << "Enter check color state";
        } );

    connect(
        state,
        & QState::exited,
        this,
        [ this ]()
        {
            const auto value = _colorButton->digitalRead();

            if( DigitalValue::OFF == value )
            {
                _color = Color::Blue;
            }
            else
            {
                _color = Color::Yellow;
            }

            displayColor( value );

            qDebug()
                << QTime::currentTime().toString()
                << "Exit check color state. Color for the game is:"
                << _color;
        } );

    return state;
}

QState* SystemManager::createStartGameState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << QTime::currentTime().toString() << "Enter start game state";

            _gameTimer.start( GAME_DURATION );
            _aliveTimer.start( 250 ); // Start blinking led
            _funnyTimer.start( FUNNY_ACTION );

            emit readyForWar();
        } );

    return state;
}

QState* SystemManager::createRunningStratState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << QTime::currentTime().toString() << "Enter running strat state";

            if( _color == Color::Yellow )
            {
                _recallage.errorInit( 35, 0, 0 ); // TODO: Change y pos
            }
            else
            {
                _recallage.errorInit( 35, 0, 0 ); // TODO: Change y pos
            }

            emit doStrat( _color );
        } );

    return state;
}

// Final state
QState* SystemManager::createStopGameState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << QTime::currentTime().toString() << "Enter stop state";
            emit stopped();
        } );

    return state;
}

// Fallback state when an error occured
QState* SystemManager::createErrorState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << QTime::currentTime().toString() << "Enter error state";
            // XXX: WHAT TO DO: Signal with error type
        } );

    return state;
}

// Hard stop state when AU button is pushed
QState* SystemManager::createHardStopState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            _gameTimer.stop();
            qDebug() << QTime::currentTime().toString() << "Enter hard stop state";
        } );

    return state;
}

void SystemManager::displayColor( const DigitalValue& value )
{
    if( value == DigitalValue::OFF )
    {
        _color = Color::Blue;
        _ledBlue->digitalWrite( DigitalValue::ON );
        _ledYellow->digitalWrite( DigitalValue::OFF );
    }
    else
    {
        _color = Color::Yellow;
        _ledBlue->digitalWrite( DigitalValue::OFF );
        _ledYellow->digitalWrite( DigitalValue::ON );
    }

    _colorSensor.changeTarget( _color );
}

void SystemManager::robotAlive()
{
    if( _color == Color::Blue )
    {
        _ledBlue->digitalWrite( DigitalValue::OFF );
        QThread::msleep( 250 );
        _ledBlue->digitalWrite( DigitalValue::ON );
        QThread::msleep( 250 );
    }
    else
    {
        _ledYellow->digitalWrite( DigitalValue::OFF );
        QThread::msleep( 250 );
        _ledYellow->digitalWrite( DigitalValue::ON );
        QThread::msleep( 250 );
    }
}
