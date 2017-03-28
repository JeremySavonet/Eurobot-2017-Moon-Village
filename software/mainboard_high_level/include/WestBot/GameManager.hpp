// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_GAMEMANAGER_HPP_
#define WESTBOT_GAMEMANAGER_HPP_

#include <QList>
#include <QObject>
#include <QStateMachine>
#include <QTimer>

#include "Action.hpp"
#include "Common.hpp"
#include "Input.hpp"

class QState;
class QString;

namespace WestBot {

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager( Input::Ptr startButton,
                 Input::Ptr colorButton,
                 Input::Ptr stopButton,
                 QObject* parent = nullptr );

    ~GameManager() override;

    void start();
    void stop();

    void pushAction( const Action::Ptr& action );
    void clearActionQueue();

signals:
    void started();
    void stopped();
    void error( const QString& error );
    void doFunnyAction();
    void systemInitialized();

    void executeAction();
    void actionQueueCleared();

    void onActionSuccess();
    void onActionError();

    void funnyActionDone();

private:
    void createStateMachine();

    // States before the game start (init, check color, tirette)
    QState* createInitialState( QState* parent );
    QState* createCheckGameColorState( QState* parent );
    QState* createStartGameState( QState* parent );

    // States to manage game action execution
    QState* createWaitForActionState( QState* parent );
    QState* createExecuteActionState( QState* parent );
    QState* createCancelActionState( QState* parent );

    // Last action of the game: Bonus +20pts.
    // We go to that state when timer reach 90s.
    QState* createFunnyActionState( QState* parent );

    // Final state
    QState* createStopGameState( QState* parent );

    // Fallback state when an error occured
    QState* createErrorState( QState* parent );

private:
    QStateMachine _stateMachine;
    QTimer _gameTimer;
    QTimer _actionTimeoutTimer;
    QList< Action::Ptr > _actions;
    Input::Ptr _startButton;
    Input::Ptr _colorButton;
    Input::Ptr _stopButton;
    Color _color;
};

}

#endif // WESTBOT_GAMEMANAGER_HPP_
