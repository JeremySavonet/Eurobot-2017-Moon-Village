// A little program to test mmap with cpp

#include <iostream>

#include "Led.hpp"

using namespace std;

int main( int argc, char** argv )
{
    Led led;

    cout << "Hello world" << endl;

    led.turnOn( 1 );

    while( true )
    {
        // WAIT
    }

    return 0;
}
