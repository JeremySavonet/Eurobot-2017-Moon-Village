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

const Layer1Config_t& Configuration::layer1Configuration() const
{
    return _layer1Configuration;
}

const Layer2Config_t& Configuration::layer2Configuration() const
{
    return _layer2Configuration;
}

const Layer3Config_t& Configuration::layer3Configuration() const
{
    return _layer3Configuration;
}
