// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_SYSTEMMANAGER_HPP_
#define WESTBOT_SYSTEMMANAGER_HPP_

#include "GameManager.hpp"

namespace WestBot {

class SystemManager
{
public:
    enum class SystemMode
    {
        Free, //No PID -> register = 0x00
        DistanceOnly, //PIDD -> register = 0x01
        AngleOnly, //PIDA -> register = 0x02
        Full, //All PID -> register = 0x03
        Unused // 0xFF : Unused for now
    };

    SystemManager();
    ~SystemManager();

    void init();

    void setMode( SystemMode mode );
    SystemMode mode() const;

private:
    GameManager _gameManager;
    SystemMode _systemMode;
};

}

#endif // WESTBOT_SYSTEMMANAGER_HPP_
