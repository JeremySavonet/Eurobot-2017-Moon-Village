// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <WestBot/ItemRegister.hpp>

using namespace WestBot;

ItemRegister::ItemRegister( Memory& layer, int offset, int size )
    : _layer( layer )
    , _offset( offset )
    , _size( size )
{
}

void ItemRegister::write( uint32_t data )
{
    _layer.write( _offset, _size, data );
}

void ItemRegister::writeFloat( float data )
{
    uint32_t* tmp = ( uint32_t* )( & data );
    _layer.write( _offset, _size, *tmp );
}

uint32_t ItemRegister::read() const
{
    return _layer.read( _offset, _size );
}

float ItemRegister::readFloat() const
{
    uint32_t val = _layer.read( _offset, _size );
    float* tmp = ( float* )( & val );
    return *tmp;
}
