#define F_CPU 1000000UL
#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "wire.h"
#include "WString.h"

void delayms(uint16_t millis) {
    //uint16_t loop;
    while ( millis ) {
        _delay_ms(1);
        millis--;
    }
}

int main(void) {
    //  DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    // while(1) {
    //   PORTB &= ~(1<<PB0); /* LED on */
    //   delayms(100);
    //   PORTB |= 1<<PB0; /* LED off */
    //   delayms(900);
    // }
    //
    
    TwoWire wire;
    String strung; 

    wire.begin();        // join i2c bus (address optional for master)

    while(true)
    {
        wire.requestFrom(2, 6);    // request 6 bytes from slave device #2

        while(wire.available())    // slave may send less than requested
        {
            char c = wire.read(); // receive a byte as character
            strung += c;
        }

        return 0;
    }

}
