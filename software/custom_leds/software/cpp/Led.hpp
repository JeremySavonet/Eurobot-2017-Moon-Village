
#ifndef LED_HPP
#define LED_HPP

class Led
{
public:
    Led();

    void turnOn( int ledId );
    void turnOff( int ledId );
    void reset();

private:
    uint32_t *custom_led_map;
    void *lw_bridge_map;
    int devmem_fd;
};

#endif
