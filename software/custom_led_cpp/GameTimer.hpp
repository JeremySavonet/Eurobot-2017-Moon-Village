// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_GAMETIMER_HPP_
#define WESTBOT_GAMETIMER_HPP_

#include <QObject>

class QTimer;

namespace WestBot {

class GameTimer : public QObject
{
    Q_OBJECT

public:
    GameTimer( int duration = 90, QObject* parent = nullptr ); // 90s

    void startGame();
    void stopGame();

public slots:
    void endGame();

signals:
    void gameStarted();
    void gameEnded();

private:
    QTimer* _timer;
    int _duration;
};

}

#endif // WESTBOT_GAMETIMER_HPP_
