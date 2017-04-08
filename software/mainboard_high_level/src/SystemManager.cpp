// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QState>
#include <QString>

#include <WestBot/FunnyAction.hpp>
#include <WestBot/SystemManager.hpp>

using namespace WestBot;

namespace
{
    const int GAME_DURATION = 10 * 1000; // 90s
    const int ACTION_TIMEOUT = 500; // 500ms
}

SystemManager::SystemManager( Hal& hal, QObject* parent )
    : QObject( parent )
    , _hal( hal )
    , _stateMachine( this )
    , _startButton( new Input(_hal.itemWithId( "IN1"), "Tirette" ) )
    , _colorButton( new Input(_hal.itemWithId( "IN2"), "Color" ) )
    , _stopButton( new Input( _hal.itemWithId( "IN3"), "AU" ) )
    , _color( Color::Unknown )
    , _systemMode( SystemManager::SystemMode::Full )
{
    _gameTimer.setSingleShot( true );

    connect(
        & _actionTimeoutTimer,
        & QTimer::timeout,
        this,
        [ this ]()
        {
            if( ! _actions.isEmpty() )
            {
                _actions.first()->setState( Action::State::InError );
                _actions.removeFirst();
                emit onActionError();
            }
        } );

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
            if( value == DigitalValue::ON )
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

void SystemManager::pushAction( const Action::Ptr& action )
{
    qDebug() << "Pushing action:" << action->name() << "in the queue";

    _actions.append( action );
    emit executeAction();
}

void SystemManager::clearActionQueue()
{
    qDebug() << "Clearing action queue: size =" << _actions.size();

    for( const auto& action : _actions )
    {
        action->setState( Action::State::Flushed );
    }

    _actions.clear();
    emit actionQueueCleared();
}

void SystemManager::setMode( SystemManager::SystemMode mode )
{
    _systemMode = mode;
}

SystemManager::SystemMode SystemManager::mode() const
{
    return _systemMode;
}

// Private methods
void SystemManager::createStateMachine()
{
    QState* initialState = createInitialState( & _stateMachine );
    QState* checkGameColorState = createCheckGameColorState( & _stateMachine );
    QState* startGameState = createStartGameState( & _stateMachine );

    QState* waitForActionState = createWaitForActionState( & _stateMachine );
    QState* executeActionState = createExecuteActionState( & _stateMachine );
    QState* cancelActionState = createCancelActionState( & _stateMachine );

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

    startGameState->addTransition( waitForActionState );

    waitForActionState->addTransition(
        this,
        & SystemManager::executeAction,
        executeActionState );

    executeActionState->addTransition(
        this,
        & SystemManager::onActionSuccess,
        waitForActionState );

    executeActionState->addTransition(
        this,
        & SystemManager::onActionError,
        cancelActionState );

    cancelActionState->addTransition( waitForActionState );

    // Fallback funny action
    waitForActionState->addTransition(
        this,
        & SystemManager::doFunnyAction,
        funnyActionState );

    executeActionState->addTransition(
        this,
        & SystemManager::doFunnyAction,
        funnyActionState );

    funnyActionState->addTransition(
        this,
        & SystemManager::funnyActionDone,
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

    waitForActionState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    executeActionState->addTransition(
        this,
        & SystemManager::error,
        errorState );

    cancelActionState->addTransition(
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

    waitForActionState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    executeActionState->addTransition(
        this,
        & SystemManager::hardStop,
        hardStopState );

    cancelActionState->addTransition(
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


// States to manage game action execution
QState* SystemManager::createWaitForActionState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << "Enter in wait for action state";

            _actionTimeoutTimer.stop();

            // If the queue is not empty, directly execute the next action
            // If not, wait strategyManager to pushBack an action.
            if( ! _actions.isEmpty() )
            {
                emit executeAction();
            }
        } );

    connect(
        state,
        & QState::exited,
        this,
        [ this ]()
        {
            qDebug() << "Exit wait for action state";

            if( ! _actions.isEmpty() )
            {
                _actions.first()->setState( Action::State::Running );
            }
        } );


    return state;
}

QState* SystemManager::createExecuteActionState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << "Enter in execute action state";

            if( _actions.isEmpty() )
            {
                emit onActionError();
                return;
            }

            const auto& action = _actions.first();
            connect(
                action.get(),
                & Action::complete,
                this,
                [ this, action ]() mutable
                {
                    if( action->hasError() )
                    {
                        emit onActionError();
                        return;
                    }

                    _actions.removeFirst();

                    emit onActionSuccess();

                } );

            action->execute();

            // Start  action timeout
            _actionTimeoutTimer.start( ACTION_TIMEOUT );
        } );

    return state;
}

QState* SystemManager::createCancelActionState( QState* parent )
{
    QState* state = new QState( parent );

    connect(
        state,
        & QState::entered,
        this,
        [ this ]()
        {
            qDebug() << "Enter in cancel action state";

            if( ! _actions.isEmpty() )
            {
                clearActionQueue();
            }
        } );

    return state;
}

// Last action of the game: Bonus +20pts.
// We go to that state when game timer reach GAME_DURATION.
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
            const auto funnyAction = std::make_shared< FunnyAction >();

            connect(
                funnyAction.get(),
                & Action::complete,
                this,
                & SystemManager::funnyActionDone );

            funnyAction->execute();
        } );

    connect(
        state,
        & QState::exited,
        this,
        []()
        {
            qDebug() << "Exit funny action state. END OF THE GAME";
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
        []()
        {
            qDebug() << "Enter stop state";
            // XXX: Deinit all peripherals and clear what is supposed to be cleared
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

            _actionTimeoutTimer.stop();

            if( ! _actions.isEmpty() )
            {
                clearActionQueue();
            }
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

