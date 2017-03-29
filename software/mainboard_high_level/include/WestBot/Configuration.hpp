// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_CONFIGURATION_HPP_
#define WESTBOT_CONFIGURATION_HPP_

#include <QObject>
#include <QString>

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

signals:
    void configurationChanged();

private:
    QFileSystemWatcher* _watcher;
    QString _configurationFilePath;
};

}

#endif // WESTBOT_CONFIGURATION_HPP_
