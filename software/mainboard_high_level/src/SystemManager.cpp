// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QState>
#include <QString>

#include <WestBot/SystemManager.hpp>

using namespace WestBot;

namespace
{
    const int GAME_DURATION = 10 * 1000; // 90s
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
    , _color( Color::Unknown )
    , _systemMode( SystemManager::SystemMode::Full )
{
    _gameTimer.setSingleShot( true );

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


    connect(
        _startButton.get(),
        & Input::stateChanged,
        this,
        [ this ]( const DigitalValue& value )
        {
            if( value == DigitalValue::OFF )
            {
                emit started();
            }
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
            }
            else
            {
                emit reArming();
            }
        } );
}

SystemManager::~SystemManager()
{
    stop();
}

// Public methods
void SystemManager::init()
{
    setPidDInverted( 0 );
    setPidDKp( 2000.0 );
    setPidDKi( 0.0 );
    setPidDKd( 0.0 );
    setPidDSaturation( 25000 );

    setPidAInverted( 1 );
    setPidAKp( 500000.0 );
    setPidAKi( 0.0 );
    setPidAKd( 0.0 );
    setPidASaturation( 20000 );

    createStateMachine();
}

void SystemManager::start()
{
    _stateMachine.start();
}

void SystemManager::stop()
{
    _stateMachine.stop();
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
        this,
        & SystemManager::doFunnyAction,
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
            if( _colorButton->digitalRead() == DigitalValue::OFF )
            {
                _color = Color::Blue;
            }
            else
            {
                _color = Color::Red;
            }

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
            _gameTimer.start( GAME_DURATION );
        } );

    connect(
        state,
        & QState::exited,
        this,
        & SystemManager::readyForWar );

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
        } );
}

// Final state
QState* SystemManager::createStopGameState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        []()
        {
            qDebug() << "Enter stop state";
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
        } );

    return state;
}

void SystemManager::setPidDInverted( uint8_t inverted )
{
    _hal._pidDistanceInverted.write( inverted );
}

void SystemManager::setPidDKp( float kp )
{
    _hal._pidDistanceKp.write( kp );
}

void SystemManager::setPidDKi( float ki )
{
    _hal._pidDistanceKi.write( ki );
}

void SystemManager::setPidDKd( float kd )
{
    _hal._pidDistanceKd.write( kd );
}

void SystemManager::setPidDSaturation( uint32_t saturation )
{
    _hal._pidDistanceSaturation.write( saturation );
}

void SystemManager::setPidAInverted( uint8_t inverted )
{
    _hal._pidAngleInverted.write( inverted );
}

void SystemManager::setPidAKp( float kp )
{
    _hal._pidAngleKp.write( kp );
}

void SystemManager::setPidAKi( float ki )
{
    _hal._pidAngleKp.write( ki );
}

void SystemManager::setPidAKd( float kd )
{
    _hal._pidAngleKp.write( kd );
}

void SystemManager::setPidASaturation( uint32_t saturation )
{
    _hal._pidAngleSaturation.write( saturation );
}
