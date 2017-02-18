// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <error.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "hps_0.h"

#include "Led.hpp"

using namespace std;

namespace
{
    // The start address and length of the Lightweight bridge
    const int HPS_TO_FPGA_LW_BASE = 0xFF200000;
    const int HPS_TO_FPGA_LW_SPAN = 0x0020000;
}

Led::Led()
    : custom_led_map( 0 )
    , lw_bridge_map( 0 )
    , devmem_fd( 0 )
{
    // Open up the /dev/mem device (aka, RAM)
    devmem_fd = open( "/dev/mem", O_RDWR | O_SYNC );
    if( devmem_fd < 0 )
    {
        perror( "devmem open" );
        return;
    }

    // mmap() the entire address space of the Lightweight bridge
    // so we can access our custom module
    lw_bridge_map = ( uint32_t* )mmap( NULL,
                                       HPS_TO_FPGA_LW_SPAN,
                                       PROT_READ | PROT_WRITE,
                                       MAP_SHARED,
                                       devmem_fd,
                                       HPS_TO_FPGA_LW_BASE );
    if( MAP_FAILED == lw_bridge_map )
    {
        perror( "devmem mmap" );
        close( devmem_fd );
        return;
    }

    // Set the custom_led_map to the correct offset within the RAM
    // (CUSTOM_LEDS_0_BASE is from "hps_0.h")
    custom_led_map = ( uint32_t* )( lw_bridge_map + CUSTOM_LEDS_0_BASE );
}

Led::~Led()
{
    reset();

    // Unmap everything and close the /dev/mem file descriptor
    int result = munmap( lw_bridge_map, HPS_TO_FPGA_LW_SPAN );
    if( result < 0 )
    {
        perror( "devmem munmap" );
        close( devmem_fd );
        return;
    }

    close( devmem_fd );
}

void Led::turnOn( int ledId )
{
    if( ledId == 1 )
    {
        *custom_led_map = 0x02;
    }
    else
    {
        //todo: continue
        *custom_led_map = 0xFF;
    }
}

void Led::turnOff( int ledId )
{
    if( ledId == 1 )
    {
        *custom_led_map = 0x02;
    }
    else
    {
        //todo: continue
        *custom_led_map = 0xFF;
    }
}

void Led::reset()
{
    // Turn all the LEDS off
    *custom_led_map = 0x00;
}
