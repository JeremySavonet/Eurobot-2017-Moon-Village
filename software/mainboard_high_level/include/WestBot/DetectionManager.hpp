// Copyright (c) 2017 All Rights Reserved WestBot

#ifndef WESTBOT_DETECTIONMANAGER_HPP_
#define WESTBOT_DETECTIONMANAGER_HPP_

#include <QObject>
#include <QString>

#include "Hal.hpp"
#include "ItemRegister.hpp"

class QTimer;

namespace WestBot {

class DetectionManager : public QObject
{
    Q_OBJECT

public:
    DetectionManager( const QString& name, QObject* parent = nullptr );
    ~DetectionManager() override = default;

    bool init( Hal& hal );

signals:
    void opponentDetected( bool status );

private:
    void check();

    const QString _name;
    ItemRegister::Ptr _distanceSensor;
    QTimer* _eventTimer;
    bool _opponentDetected;

};

}

#endif // WESTBOT_DETECTIONMANAGER_HPP_
