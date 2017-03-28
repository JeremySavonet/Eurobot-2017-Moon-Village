// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SYSTEMMANAGER_HPP_
#define WESTBOT_SYSTEMMANAGER_HPP_

#include "GameManager.hpp"

namespace WestBot {

class SystemManager
{
public:
    SystemManager();
    ~SystemManager();

    void init();

private:
    GameManager _gameManager;
};

}

#endif // WESTBOT_SYSTEMMANAGER_HPP_
