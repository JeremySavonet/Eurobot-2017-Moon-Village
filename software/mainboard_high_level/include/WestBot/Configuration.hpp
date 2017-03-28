// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_CONFIGURATION_HPP_
#define WESTBOT_CONFIGURATION_HPP_

#include <QObject>
#include <QString>

#include "System.hpp"

class QFileSystemWatcher;

namespace WestBot {

class Configuration : public QObject
{
    Q_OBJECT

public:
    Configuration( const QString& filename );
    ~Configuration() override;

    void load();
    void save();

    const QString& filename() const;

    const Layer1Config_t& layer1Configuration() const;
    const Layer2Config_t& layer2Configuration() const;
    const Layer3Config_t& layer3Configuration() const;

signals:
    void configurationChanged();

private:
    QFileSystemWatcher* _watcher;
    QString _configurationFilePath;
    Layer1Config_t _layer1Configuration;
    Layer2Config_t _layer2Configuration;
    Layer3Config_t _layer3Configuration;
};

}

#endif // WESTBOT_CONFIGURATION_HPP_
