// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <hps_arm.h> // For our base address

#include <WestBot/Hal.hpp>
#include <WestBot/Memory.hpp>

using namespace WestBot;

Hal::Hal()
{
}

Hal::~Hal()
{
}

void Hal::init()
{
    // Create our layer mapping
    Memory layer1( _memoryManager, PIO_N_LAYER1_BASE );
    Memory layer2( _memoryManager, PIO_N_LAYER2_BASE );
    Memory layer3( _memoryManager, PIO_N_LAYER3_BASE );

    qInfo() << "Successfully initialized Hal module";
}
