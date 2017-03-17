// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_BUTTON_HPP_
#define WESTBOT_BUTTON_HPP_

#include <memory>

#include <QObject>
#include <QString>

#include <stdint.h>

#include "MemoryManager.hpp"

namespace WestBot {

class Button : public QObject
{
    Q_OBJECT

public:
    enum class ButtonType
    {
        Tirette,
        Color,
        Au,
        Other
    };

    using Ptr = std::shared_ptr< Button >;

    Button( MemoryManager& manager,
            const QString& name,
            QObject* parent = nullptr );
    ~Button() override;

    const QString& name() const;

    void reset();

signals:
    void released();
    void pressed();

protected:
    void handleEvents();

private:
    MemoryManager _memoryManager;
    QString _name;
    uint32_t *_buttonMap;
    bool _isPressed;
};

}

#endif // WESTBOT_BUTTON_HPP_
