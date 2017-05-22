// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QState>
#include <QString>
#include <QThread>

#include <WestBot/SystemManager.hpp>

using namespace WestBot;

namespace
{
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
    , _systemMode( SystemManager::SystemMode::Full )
    , _lidar( "/dev/ttyUSB0" )
    , _detectionManager( "Opponent_detector" )
    , _positionManager( _lidar )
{
    connect(
        & _gameTimer,
        & QTimer::timeout,
        this,
        [ this ]()
        {
            qDebug() << "End of the game, time to switch to the"
                     << "funny action state";

            emit doFunnyAction();
        } );

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
                    qWarning() << "Could not start game if AU is on";
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
                qDebug() << "Opponent detected";
                emit opponentDetected();
            }
            else
            {
                qDebug() << "We are safe";
            }
        } );

    connect(
        & _positionManager,
        & PositionManager::positionUpdated,
        this,
        [ this ]( int theta, int x, int y )
        {
            qDebug()
                << "New position is: Theta:" << theta << " X:" << x << " Y:" << y;
        } );
}

SystemManager::~SystemManager()
{
    _lidar.stopMotor();
    qDebug() << "Stop motor";

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

    // Color sensor
    if( ! _colorSensor.attach( _hal ) )
    {
        qWarning() << "Failed to attach color sensor module";
        return false;
    }

    // Distance sensor
    if( ! _detectionManager.init( _hal ) )
    {
        qWarning() << "Failed to init proximity sensor module";
        return false;
    }

    if( ! _positionManager.init() )
    {
        qWarning() << "Failed to init position manager module";
        return false;
    }

    // Override output registers
    _hal._outputOverride.write( 0x01010101 );

    displayColor( _colorButton->digitalRead() );

    if( ! _lidar.connect() )
    {
        qWarning() << "Failed to connect to RPLidar";
        return false;
    }

    _gameTimer.setSingleShot( true );

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

    qDebug() << "System was reset";
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
    QState* funnyActionState = createFunnyActionState( & _stateMachine );

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
        funnyActionState );

    funnyActionState->addTransition( stopGameState );

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

    funnyActionState->addTransition(
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

    funnyActionState->addTransition(
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
            qDebug() << "Enter initial state";
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
            qDebug() << "Enter check color state";
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
            qDebug() << "Enter start game state";

            _lidar.startMotor();

            _gameTimer.start( GAME_DURATION );
            _aliveTimer.start( 250 ); // Start blinking led
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
            qDebug() << "Enter running strat state";
            emit doStrat( _color );
        } );

    return state;
}

QState* SystemManager::createFunnyActionState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << "Enter funny action state";
            emit doFunnyAction();
        } );

    connect(
        state,
        & QState::exited,
        this,
        [ this ]()
        {
            emit stopped();
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
            qDebug() << "Enter stop state";
            _lidar.stopMotor();
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
            qDebug() << "Enter error state";
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
            qDebug() << "Enter hard stop state";

            _lidar.stopMotor();
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
