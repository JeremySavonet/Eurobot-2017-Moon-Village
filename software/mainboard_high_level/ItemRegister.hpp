// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ITEMREGISTER_HPP_
#define WESTBOT_ITEMREGISTER_HPP_

#include "Memory.hpp"

namespace WestBot {

class ItemRegister
{
public:
    ItemRegister( Memory& layer, int offset, int size );

    void writeFloat( float data );
    void write( uint32_t data );

    uint32_t read() const;
    float readFloat() const;

private:
    Memory _layer;
    int _offset;
    int _size;
};

}

#endif // WESTBOT_ITEMREGISTER_HPP_
