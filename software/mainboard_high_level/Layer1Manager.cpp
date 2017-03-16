// Copyright (c) 2016-2017 All Rights Reserved WestBot


// For Debug purpose only
#define DEBUG

#ifdef DEBUG

#include <QDebug>

#endif

#include "Layer1Manager.hpp"

using namespace WestBot;

Layer1Manager::Layer1Manager( MemoryManager& manager, uint32_t baseAddress )
    : _memoryManager( manager )
{
    _layerMap = reinterpret_cast< Layer1* >(
        _memoryManager.mapModuleToMemory( baseAddress ) );
}

Layer1Manager::~Layer1Manager()
{
    reset();
}

void Layer1Manager::reset()
{
    _layerMap = NULL;
}

uint32_t Layer1Manager::read( uint32_t offset, uint32_t size ) const
{
#ifdef DEBUG
    qDebug() << "WANT TO READ AT OFFSET:" << offset
             << "SIZE: " << size;
#endif

    uint32_t data = 0;

    switch( size )
    {
    case 8:
        data = *( ( uint8_t* )( &_layerMap[ offset ] ) );
        break;

    case 16:
        data = *( ( uint16_t* )( &_layerMap[ offset ] ) );
        break;

    case 32:
        data = *( ( uint32_t* )( &_layerMap[ offset ] ) );
        break;

    default:
        qWarning() << "Try to read invalid size:" << size
                   << "at offset:" << offset;
        break;
    }

    return data;
}

void Layer1Manager::write( uint32_t offset, uint32_t size, uint32_t data )
{
#ifdef DEBUG
    qDebug() << "WANT TO WRITE DATA:" << data << "AT OFFSET:" << offset
             << "SIZE: " << size;
#endif

    switch( size )
    {
    case 8:
        *( ( uint8_t* )( &_layerMap[ offset ] ) ) = data;
        break;

    case 16:
        *( ( uint16_t* )( &_layerMap[ offset ] ) ) = data;
        break;

    case 32:
        *( ( uint32_t* )( &_layerMap[ offset ] ) ) = data;
        break;

    default:
        qWarning() << "Try to write invalid size:" << size
                   << "at offset:" << offset;
        break;
    }
}
