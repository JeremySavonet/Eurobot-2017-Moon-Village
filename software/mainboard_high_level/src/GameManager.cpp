// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <memory>

#include <QState>
#include <QString>

#include <WestBot/FunnyAction.hpp>
#include <WestBot/GameManager.hpp>

using namespace WestBot;

namespace
{
    const int GAME_DURATION = 10 * 1000; // 90s
    const int ACTION_TIMEOUT = 500; // 500ms
}

GameManager::GameManager(
    Input::Ptr startButton,
    Input::Ptr colorButton,
    Input::Ptr stopButton,
    QObject* parent )
    : QObject( parent )
    , _stateMachine( this )
    , _startButton( startButton )
    , _colorButton( colorButton )
    , _stopButton( stopButton )
    , _color( Color::Unknown )

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
                emit stopped();
            }
        } );

    createStateMachine();
}

GameManager::~GameManager()
{
    stop();
}

// Public methods
void GameManager::start()
{
    _stateMachine.start();
}

void GameManager::stop()
{
    _stateMachine.stop();
}

void GameManager::pushAction( const Action::Ptr& action )
{
    qDebug() << "Pushing action:" << action->name() << "in the queue";

    _actions.append( action );
    emit executeAction();
}

void GameManager::clearActionQueue()
{
    qDebug() << "Clearing action queue: size =" << _actions.size();

    for( const auto& action : _actions )
    {
        action->setState( Action::State::Flushed );
    }

    _actions.clear();
    emit actionQueueCleared();
}

// Private methods
void GameManager::createStateMachine()
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

    // Add state transitions
    initialState->addTransition( checkGameColorState );

    checkGameColorState->addTransition(
        this,
        & GameManager::started,
        startGameState );

    startGameState->addTransition( waitForActionState );

    waitForActionState->addTransition(
        this,
        & GameManager::executeAction,
        executeActionState );

    executeActionState->addTransition(
        this,
        & GameManager::onActionSuccess,
        waitForActionState );

    executeActionState->addTransition(
        this,
        & GameManager::onActionError,
        cancelActionState );

    cancelActionState->addTransition( waitForActionState );

    // Fallback funny action
    waitForActionState->addTransition(
        this,
        & GameManager::doFunnyAction,
        funnyActionState );

    executeActionState->addTransition(
        this,
        & GameManager::doFunnyAction,
        funnyActionState );

    funnyActionState->addTransition(
        this,
        & GameManager::funnyActionDone,
        stopGameState );

    // Rearm the system for an other game
    stopGameState->addTransition( initialState );

    // Fallback error state
    checkGameColorState->addTransition(
        this,
        & GameManager::error,
        errorState );

    startGameState->addTransition(
        this,
        & GameManager::error,
        errorState );

    waitForActionState->addTransition(
        this,
        & GameManager::error,
        errorState );

    executeActionState->addTransition(
        this,
        & GameManager::error,
        errorState );

    cancelActionState->addTransition(
        this,
        & GameManager::error,
        errorState );

    funnyActionState->addTransition(
        this,
        & GameManager::error,
        errorState );

    stopGameState->addTransition(
        this,
        & GameManager::error,
        errorState );

    _stateMachine.setInitialState( initialState );
}

// States before the game start (init, check color, tirette)
QState* GameManager::createInitialState( QState* parent )
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

QState* GameManager::createCheckGameColorState( QState* parent )
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

QState* GameManager::createStartGameState( QState* parent )
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

    return state;
}


// States to manage game action execution
QState* GameManager::createWaitForActionState( QState* parent )
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

QState* GameManager::createExecuteActionState( QState* parent )
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

QState* GameManager::createCancelActionState( QState* parent )
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
QState* GameManager::createFunnyActionState( QState* parent )
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
                & GameManager::funnyActionDone );

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
QState* GameManager::createStopGameState( QState* parent )
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
QState* GameManager::createErrorState( QState* parent )
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
