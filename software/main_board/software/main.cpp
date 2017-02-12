// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <iostream>
#include <stdint.h>
#include <unistd.h>

#include "include/WestBot/Memory/MemoryManager.hpp"
#include "include/WestBot/Motor/DcMotorManager.hpp"
#include "include/WestBot/Motor/QeiManager.hpp"

using namespace WestBot;

int main( void )
{
    // Init manager for Fpga memory access
    MemoryManager memoryManager;

    // Init managers for motor controls
    DcMotorManager motorManager( memoryManager );
    QeiManager qeiManager( memoryManager );

    std::cout << "Here we start!" << std::endl;

    motorManager.setPwm( 0, 0, 50 ); // set pwm M0 to 50%

    int32_t enc0;
    int32_t enc1;

    while( true )
    {
        enc0 = qeiManager.getEncoderValue( 0 ); // Get M0 encoder values
        enc1 = qeiManager.getEncoderValue( 1 ); // Get M1 encoder values

        std::cout << "Enc0: " << enc0 << " Enc1: " << enc1 << std::endl;

        // Wait 10us
        usleep( 10 );
    }

    return 0;
}
