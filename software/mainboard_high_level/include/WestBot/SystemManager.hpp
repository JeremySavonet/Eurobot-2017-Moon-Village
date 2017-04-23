// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SYSTEMMANAGER_HPP_
#define WESTBOT_SYSTEMMANAGER_HPP_

#include <QList>
#include <QObject>
#include <QStateMachine>
#include <QTimer>

#include "Action.hpp"
#include "Common.hpp"
#include "Hal.hpp"
#include "Input.hpp"

class QState;
class QString;

namespace WestBot {

class SystemManager : public QObject
{
    Q_OBJECT

public:
    enum class SystemMode
    {
        Free, //No PID -> register = 0x00
        DistanceOnly, //PIDD -> register = 0x01
        AngleOnly, //PIDA -> register = 0x02
        Full, //All PID -> register = 0x03
        Unused // 0xFF : Unused for now
    };

    SystemManager( Hal& hal, QObject* parent = nullptr );

    ~SystemManager() override;

    void init();

    void start();
    void stop();

    void pushAction( const Action::Ptr& action );
    void clearActionQueue();

    void setMode( SystemMode mode );
    SystemMode mode() const;

signals:
    void started();
    void stopped();
    void error( const QString& error );
    void doFunnyAction();
    void systemInitialized();

    void readyForWar();

    void executeAction();
    void actionQueueCleared();

    void onActionSuccess();
    void onActionError();

    void funnyActionDone();

    void hardStop();
    void reArming();

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

    // Hard stop state when AU button is pushed
    QState* createHardStopState( QState* parent );

    void setPidDInverted( uint8_t inverted );
    void setPidDKp( float kp );
    void setPidDKi( float ki );
    void setPidDKd( float kd );
    void setPidDSaturation( uint32_t saturation );

    void setPidAInverted( uint8_t inverted );
    void setPidAKp( float kp );
    void setPidAKi( float ki );
    void setPidAKd( float kd );
    void setPidASaturation( uint32_t saturation );

private:
    Hal _hal;
    QStateMachine _stateMachine;
    QTimer _gameTimer;
    QTimer _actionTimeoutTimer;
    QList< Action::Ptr > _actions;
    Input::Ptr _startButton;
    Input::Ptr _colorButton;
    Input::Ptr _stopButton;
    Color _color;
    SystemMode _systemMode;
};

}

#endif // WESTBOT_SYSTEMMANAGER_HPP_
