#define F_CPU 1000000UL
//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "TinyWireM.h"

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
    
    USI_TWI wire;
    uint8_t arr[16]; 

    wire.begin();        // join i2c bus (address optional for master)

    while(true)
    {
        wire.requestFrom(2, 6);    // request 6 bytes from slave device #2

        uint8_t i = 0;
        uint8_t c = 0;
        while(wire.available())    // slave may send less than requested
        {
            c = wire.read(); // receive a byte as character
            arr[i] = c;
            i++;
        }

        return 0;
    }

}
