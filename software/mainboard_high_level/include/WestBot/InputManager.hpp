// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_INPUTMANAGER_HPP_
#define WESTBOT_INPUTMANAGER_HPP_

#include <memory>

#include <QFlags>
#include <QObject>

namespace WestBot {

class InputManager : public QObject
{
    Q_OBJECT

public:
    enum class InputType
    {
        Tirette,
        Color,
        Au,
        Other
    };

    Q_DECLARE_FLAGS


    InputManager();
    ~InputManager() override;

protected:
    void handleEvents();
};

}

#endif // WESTBOT_INPUTMANAGER_HPP_
