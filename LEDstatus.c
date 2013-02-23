//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "LEDstatus.h"
#include "pindefs.h"

/*
   void delayms(uint16_t millis) 
   {
//uint16_t loop;
while ( millis ) {
_delay_ms(1);
millis--;
}
}
*/

void LEDon()
{
    STATUSIOPORT |= 1<<STATUSVCC; // Set PA5 to output.  This will be our current source.
    STATUSPORT |= 1<<STATUSVCC; // Source current from PA5
    STATUSIOPORT |= 1<<STATUSLED; // set PA7 to output.  "output" means "sink current"
    STATUSPORT &= ~(1<<STATUSLED); /* LED on */
}

void LEDoff()
{
    STATUSPORT |= 1<<STATUSLED; /* LED off */
}


void LEDflashSignal()
{
    STATUSIOPORT |= 1<<STATUSVCC; // Set PA5 to output.  This will be our current source.
    STATUSPORT |= 1<<STATUSVCC; // Source current from PA5

    STATUSIOPORT |= 1<<STATUSLED; // set PA7 to output.  "output" means "sink current"
    STATUSPORT &= ~(1<<STATUSLED); /* LED on */
    _delay_ms(500);
    STATUSPORT |= 1<<STATUSLED; /* LED off */
    _delay_ms(500);
}


void LEDflashData(unsigned char data)
{
    STATUSIOPORT |= 1<<STATUSVCC; // Set PA5 to output.  This will be our current source.
    STATUSPORT |= 1<<STATUSVCC; // Source current from PA5

    STATUSIOPORT |= 1<<STATUSLED; // set PA7 to output.  "output" means "sink current"
    unsigned char i;
    LEDflashAlert();
    for(i = 8 ; i>0 ; i--) 
    {
        STATUSPORT &= ~(1<<STATUSLED); /* LED on */
        _delay_ms(100);
        STATUSPORT |= 1<<STATUSLED; /* LED off */
        _delay_ms(100);


        STATUSPORT &= ~(1<<STATUSLED); /* LED on */
        if((data & 0x80)) 
            _delay_ms(500);
        else
            _delay_ms(100);
        STATUSPORT |= 1<<STATUSLED; /* LED off */

        data <<= 1;

        _delay_ms(900);
    }
}


void LEDflashAlert()
{
    STATUSIOPORT |= 1<<STATUSVCC; // Set PA5 to output.  This will be our current source.
    STATUSPORT |= 1<<STATUSVCC; // Source current from PA5

    STATUSIOPORT |= 1<<STATUSLED; // set PA7 to output.  "output" means "sink current"
    STATUSPORT |= 1<<STATUSLED; /* LED off */
    _delay_ms(100);
    STATUSPORT &= ~(1<<STATUSLED); /* LED on */
    _delay_ms(100);
    STATUSPORT |= 1<<STATUSLED; /* LED off */
    _delay_ms(100);
    STATUSPORT &= ~(1<<STATUSLED); /* LED on */
    _delay_ms(100);
    STATUSPORT |= 1<<STATUSLED; /* LED off */
    _delay_ms(100);
    STATUSPORT &= ~(1<<STATUSLED); /* LED on */
    _delay_ms(100);
    STATUSPORT |= 1<<STATUSLED; /* LED off */
    _delay_ms(1000);
}
