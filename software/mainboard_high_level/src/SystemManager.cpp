// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/SystemManager.hpp>

using namespace WestBot;

SystemManager::SystemManager()
    : _systemMode( SystemManager::SystemMode::Full )
{
}

SystemManager::~SystemManager()
{
}

void SystemManager::init()
{
    _gameManager.start();

    qInfo() << "Successfully initialized system";
}

void SystemManager::setMode( SystemManager::SystemMode mode )
{
    _systemMode = mode;
}

SystemManager::SystemMode SystemManager::mode() const
{
    return _systemMode;
}
