//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "LEDstatus.h"

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
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB &= ~(1<<PB0); /* LED on */
}

void LEDoff()
{
    PORTB |= 1<<PB0; /* LED off */
}


void LEDflashSignal()
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB &= ~(1<<PB0); /* LED on */
    _delay_ms(500);
    PORTB |= 1<<PB0; /* LED off */
    _delay_ms(500);
}


void LEDflashData(unsigned char data)
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    unsigned char i;
    LEDflashAlert();
    for(i = 8 ; i>0 ; i--) 
    {
        PORTB &= ~(1<<PB0); /* LED on */
        _delay_ms(100);
        PORTB |= 1<<PB0; /* LED off */
        _delay_ms(100);


        PORTB &= ~(1<<PB0); /* LED on */
        if((data & 0x80)) 
            _delay_ms(500);
        else
            _delay_ms(100);
        PORTB |= 1<<PB0; /* LED off */

        data <<= 1;

        _delay_ms(900);
    }
}


void LEDflashAlert()
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB |= 1<<PB0; /* LED off */
    _delay_ms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    _delay_ms(100);
    PORTB |= 1<<PB0; /* LED off */
    _delay_ms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    _delay_ms(100);
    PORTB |= 1<<PB0; /* LED off */
    _delay_ms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    _delay_ms(100);
    PORTB |= 1<<PB0; /* LED off */
    _delay_ms(1000);
}
