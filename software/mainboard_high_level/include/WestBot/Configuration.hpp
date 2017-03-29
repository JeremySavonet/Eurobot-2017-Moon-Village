// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_CONFIGURATION_HPP_
#define WESTBOT_CONFIGURATION_HPP_

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

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
    
    const QMap< QString, QVariant >& settings() const;
    QMap< QString, QVariant >& mutableSettings();
    
signals:
    void configurationChanged();

private:
    QFileSystemWatcher* _watcher;
    QString _configurationFilePath;
    QMap< QString, QVariant > _settings;
};

}

#endif // WESTBOT_CONFIGURATION_HPP_
