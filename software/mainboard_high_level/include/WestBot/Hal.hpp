// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_HAL_HPP_
#define WESTBOT_HAL_HPP_

#include <QMap>
#include <QString>

#include "ItemRegister.hpp"
#include "MemoryManager.hpp"
#include <WestBot/Memory.hpp>

namespace WestBot {

class Hal
{
public:
    Hal();
    ~Hal();

    void init();

    ItemRegister::Ptr itemWithId( const QString& id );
    QMap< QString, ItemRegister::Ptr >& items();

private:
    MemoryManager _memoryManager;
    Memory _layer1;
    Memory _layer2;
    Memory _layer3;
    QMap< QString, ItemRegister::Ptr > _registersMap;
};

}

#endif // WESTBOT_HAL_HPP_
