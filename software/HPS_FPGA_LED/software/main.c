// Std libs
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "hwlib.h"
#include "socal/alt_gpio.h"
#include "socal/hps.h"
#include "socal/socal.h"

// User includes
#include "hps_0.h"
#include "led.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

volatile unsigned long *h2p_lw_led_addr = NULL;

// Entry point
int main( int argc, char **argv )
{
    void *virtual_base;
    int fd;
    int i;

    printf( "Hello World\r\n Led Blinker Project\r\n" );

    // map the address space for the LED registers into user space so we can
    // interact with them.
    // We will actually map in the entire CSR spqn of the HPS since we want
    // to access various registers within that span.

    if( -1 == ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) )
    {
        printf( "ERROR: could not open \"/dev/mem\"...\r\n" );
        return( 1 );
    }

    virtual_base = mmap( NULL,
                         HW_REGS_SPAN,
                         ( PROT_READ | PROT_WRITE ),
                         MAP_SHARED,
                         fd,
                         HW_REGS_BASE );

    if( MAP_FAILED == virtual_base )
    {
        printf( "ERROR: mmap() failed...\r\n" );
        return( 1 );
    }

    h2p_lw_led_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + LED_PIO_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

    while( 1 )
    {
        printf( "LED ON \r\n" );

        for( i = 0; i <= 8; i++ )
        {
            LEDR_LightCount( i );
            usleep( 100 * 1000 );
        }

        printf( "LED OFF \r\n" );
        for( i = 0; i <= 8; i++ )
        {
            LEDR_OffCount( i );
            usleep( 100 * 1000 );
        }
    }

    if( 0 != munmap( virtual_base, HW_REGS_SPAN ) )
    {
        printf( "ERROR: munmap() failed...\r\n" );
        close( fd );
        return( 1 );
    }

    close( fd );
    return( 0 );
}
