// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <stdint.h>

#ifndef LED_HPP
#define LED_HPP

class Led
{
public:
    Led(); // Default constructor
    ~Led(); // Destructor

    void turnOn( int ledId );
    void turnOff( int ledId );
    void reset();

private:
    uint32_t *custom_led_map;
    void *lw_bridge_map;
    int devmem_fd;
};

#endif
