// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QFileSystemWatcher>
#include <QSettings>
#include <QStringList>

#include <WestBot/Configuration.hpp>

using namespace WestBot;

Configuration::Configuration( const QString& filename )
    : _configurationFilePath( filename )
{
    _watcher = new QFileSystemWatcher( this );
    _watcher->addPath( _configurationFilePath );

    connect(
        _watcher,
        & QFileSystemWatcher::fileChanged,
        this,
        [ this ] ( const QString& filename )
        {
            Q_UNUSED( filename )
            emit configurationChanged();
        } );
}

Configuration::~Configuration()
{
    save();
    _watcher->removePath( _configurationFilePath );
}

const QString& Configuration::filename() const
{
    return _configurationFilePath;
}

void Configuration::load()
{
    // Open the ini configuration file.
    QSettings settings( _configurationFilePath, QSettings::IniFormat );

    // Purge settings
    _settings.clear();

    const QStringList& childKeys = settings.allKeys();
    for( const auto& key : childKeys )
    {
        _settings.insert( key, settings.value( key ) );
    }
}

void Configuration::save()
{
    QSettings settings( _configurationFilePath, QSettings::IniFormat );

    for( auto it = _settings.constBegin(); it != _settings.constEnd(); ++it )
    {
        settings.setValue( it.key(), it.value() );
    }
}

const QHash< QString, QVariant >& Configuration::settings() const
{
    return _settings;
}

QHash< QString, QVariant >& Configuration::mutableSettings()
{
    return _settings;
}
