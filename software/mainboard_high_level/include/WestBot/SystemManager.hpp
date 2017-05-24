// COPYRIGHT (C) 2016-2017 ALL RIGHTS RESERVED WESTBot

#ifndef WESTBOT_SYSTEMMANAGER_HPP_
#define WESTBOT_SYSTEMMANAGER_HPP_

#include <QList>
#include <QObject>
#include <QStateMachine>
#include <QTimer>

#include "Carrousel.hpp"
#include "ColorSensor.hpp"
#include "Common.hpp"
#include "DetectionManager.hpp"
#include "Hal.hpp"
#include "Input.hpp"
#include "Output.hpp"
#include "Recallage.hpp"
#include "RPLidar.hpp"
#include "Servo.hpp"
#include "StrategyManager.hpp"
#include "TrajectoryManager.hpp"
#include "Turbine.hpp"

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

    bool init();

    void start();
    void stop();
    void reset();

    void setMode( SystemMode mode );
    SystemMode mode() const;

    const Color& color() const;

signals:
    void started();
    void stopped();
    void error( const QString& error );
    void doFunnyAction();
    void systemInitialized();

    void readyForWar();

    void hardStop();
    void reArming();

    void doStrat( const Color& color );
    void opponentDetected();

private:
    void createStateMachine();

    // States before the game start (init, check color, tirette)
    QState* createInitialState( QState* parent );
    QState* createCheckGameColorState( QState* parent );
    QState* createStartGameState( QState* parent );

    QState* createRunningStratState( QState* parent );
    QState* createFunnyActionState( QState* parent );

    // Final state
    QState* createStopGameState( QState* parent );

    // Fallback state when an error occured
    QState* createErrorState( QState* parent );

    // Hard stop state when AU button is pushed
    QState* createHardStopState( QState* parent );

    void robotAlive();

private:
    void displayColor( const DigitalValue& value );

    Hal _hal;
    QStateMachine _stateMachine;
    QTimer _gameTimer;
    QTimer _aliveTimer;
    Input::Ptr _startButton;
    Input::Ptr _colorButton;
    Input::Ptr _stopButton;
    Output::Ptr _ledYellow;
    Output::Ptr _ledBlue;
    Color _color;
    ColorSensor _colorSensor;
    Servo _armRight;
    Servo _armLeft;
    Servo _ejector;
    Servo _unblock;
    Turbine _turbine;
    Carrousel _carrousel;
    TrajectoryManager _trajectoryManager;
    SystemMode _systemMode;
    RPLidar _lidar;
    DetectionManager _detectionManager;
    Recallage _recallage;
    StrategyManager _strategyManager;
};

}

#endif // WESTBOT_SYSTEMMANAGER_HPP_
