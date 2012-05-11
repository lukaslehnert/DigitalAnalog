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
    DDRA |= 1<<PA7; // set PA7 to output.  "output" means "sink current"
    PORTA &= ~(1<<PA7); /* LED on */
}

void LEDoff()
{
    PORTA |= 1<<PA7; /* LED off */
}


void LEDflashSignal()
{
    DDRA |= 1<<PA7; // set PA7 to output.  "output" means "sink current"
    PORTA &= ~(1<<PA7); /* LED on */
    _delay_ms(500);
    PORTA |= 1<<PA7; /* LED off */
    _delay_ms(500);
}


void LEDflashData(unsigned char data)
{
    DDRA |= 1<<PA7; // set PA7 to output.  "output" means "sink current"
    unsigned char i;
    LEDflashAlert();
    for(i = 8 ; i>0 ; i--) 
    {
        PORTA &= ~(1<<PA7); /* LED on */
        _delay_ms(100);
        PORTA |= 1<<PA7; /* LED off */
        _delay_ms(100);


        PORTA &= ~(1<<PA7); /* LED on */
        if((data & 0x80)) 
            _delay_ms(500);
        else
            _delay_ms(100);
        PORTA |= 1<<PA7; /* LED off */

        data <<= 1;

        _delay_ms(900);
    }
}


void LEDflashAlert()
{
    DDRA |= 1<<PA7; // set PA7 to output.  "output" means "sink current"
    PORTA |= 1<<PA7; /* LED off */
    _delay_ms(100);
    PORTA &= ~(1<<PA7); /* LED on */
    _delay_ms(100);
    PORTA |= 1<<PA7; /* LED off */
    _delay_ms(100);
    PORTA &= ~(1<<PA7); /* LED on */
    _delay_ms(100);
    PORTA |= 1<<PA7; /* LED off */
    _delay_ms(100);
    PORTA &= ~(1<<PA7); /* LED on */
    _delay_ms(100);
    PORTA |= 1<<PA7; /* LED off */
    _delay_ms(1000);
}
