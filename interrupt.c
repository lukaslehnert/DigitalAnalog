#define F_CPU 1000000UL
//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "LEDstatus.h"
#include "interrupt.h"




void INT_attachInterruptA(uint8_t pin)
{
    PORTA |= (1<<pin);
    PCMSK0 |= (1<<pin);
    GIMSK |= (1<<PCIE0);
}


void INT_attachInterruptB(uint8_t pin)
{
    PORTB |= (1<<pin);
    PCMSK1 |= (1<<pin);
    GIMSK |= (1<<PCIE1);
}


