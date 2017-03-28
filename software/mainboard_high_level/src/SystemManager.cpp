// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/SystemManager.hpp>

using namespace WestBot;

SystemManager::SystemManager()
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
