// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include "Memory.hpp"

using namespace WestBot;

Memory::Memory( MemoryManager& manager, uint32_t baseAddress )
    : _memoryManager( manager )
{
    _layerMap = reinterpret_cast< Layer_t* >(
        _memoryManager.mapModuleToMemory( baseAddress ) );
}

Memory::~Memory()
{
    reset();
}

void Memory::reset()
{
    _layerMap = NULL;
}

uint32_t Memory::read( uint32_t offset,
                       uint32_t size ) const
{
    uint32_t data = 0;

    switch( size )
    {
    case 8:
        data = *( (uint8_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) );
        break;

    case 16:
        data = *( (uint16_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) );
        break;

    case 32:
        data = *( (uint32_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) );
        break;

    default:
        qWarning() << "Try to read invalid size:" << size
                   << "at offset:" << offset;
        break;
    }

    return data;
}

void Memory::write( uint32_t offset,
                    uint32_t size,
                    uint32_t data )
{
    switch( size )
    {
    case 8:
        *( (uint8_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) ) = data;
        break;

    case 16:
        *( (uint16_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) ) = data;
        break;

    case 32:
        *( (uint32_t*)( &( ((uint8_t*)_layerMap)[ offset ] ) ) ) = data;
        break;

    default:
        qWarning() << "Try to write invalid size:" << size
                   << "at offset:" << offset;
        break;
    }
}
