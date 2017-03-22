// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>
#include <QFileSystemWatcher>
#include <QSettings>

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
    QSettings settings( _configurationFilePath, QSettings::IniFormat );
    Q_UNUSED( settings );
    // XXX: TODO get all the settings from the ini file
}

void Configuration::save()
{
    QSettings settings( _configurationFilePath, QSettings::IniFormat );
    Q_UNUSED( settings );
    // XXX: Implement
}
