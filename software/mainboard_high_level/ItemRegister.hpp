// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_ITEMREGISTER_HPP_
#define WESTBOT_ITEMREGISTER_HPP_

#include "LayerManager.hpp"

namespace WestBot {

template< typename LayerType >
class ItemRegister
{
public:
    ItemRegister( const LayerManager< LayerType >& layer,
                  uint32_t offset,
                  uint32_t size )
        : _layer( layer )
        , _offset( offset )
        , _size( size )
    {
    }

    void write( uint32_t data )
    {
        _layer.write( _offset, _size, data );
    }

    uint32_t read() const
    {
        return _layer.read( _offset, _size );
    }

private:
    LayerManager< LayerType > _layer;
    uint32_t _offset;
    uint32_t _size;
};

}

#endif // WESTBOT_ITEMREGISTER_HPP_
