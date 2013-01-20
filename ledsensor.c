
// This example shows one way of using an LED as a light sensor.
// You will need to wire up your components as such:
//
//           + digital2
//           |
//           <
//           > 100 ohm resistor
//           <
//           |
//           |
//         -----
//          / \  LED, maybe a 5mm, clear plastic is good
//         -----
//           |
//           |
//           + digital3
//
// What we are going to do is apply a positive voltage at digital2 and
// a low voltage at digital3. This is backwards for the LED, current will
// not flow and light will not come out, but we will charge up the 
// capacitance of the LED junction and the Arduino pin.
//
// Then we are going to disconnect the output drivers from digital2 and
// count how long it takes the stored charge to bleed off through the 
// the LED. The brighter the light, the faster it will bleed away to 
// digital3.
//
// Then just to be perverse we will display the brightness back on the 
// same LED by turning it on for a millisecond. This happens more often
// with brighter lighting, so the LED is dim in a dim room and brighter 
// in a bright room. Quite nice.
//
// (Though a nice idea, this implementation is flawed because the refresh
// rate gets too long in the dark and it flickers disturbingly.)


#include <avr/io.h>
#include <util/delay.h>
#include "shift.h"


#define bit_get(p,m) ((p) & (m)) 
#define BIT(x) (0x01 << (x)) 

#define POS PA1
#define NEG PA0


void bargraph(int value)
{
    int i;
    for (i=8; i>0; i--)
        SR_push0(PA2, PA4);

    for (value=value; value>0; value--)
        SR_push1(PA2, PA4);
}

void bytegraph(int value)
{
    int i;

    for (i=8; i>0; i--)
    {
        if ( (0b10000000 & value) == 0b10000000)
            SR_push1(PA2, PA4);
        else
            SR_push0(PA2, PA4);
        value = value << 1;
    }
}


int read_LED ()
{
    unsigned int j;
    unsigned int mask, notmask;


    mask = 1<<NEG;
    notmask = ~(1<<NEG);


    //Apply reverse voltage, charge up the pin and led capacitance
    ///pinMode(LED_N_SIDE,OUTPUT);
    DDRA |= 1<<NEG;
    ///pinMode(LED_P_SIDE,OUTPUT);
    DDRA |= 1<<POS;
    ///digitalWrite(LED_N_SIDE,HIGH);
    PORTA |= 1<<NEG;
    ///digitalWrite(LED_P_SIDE,LOW);
    PORTA &= ~(1<<POS);

    _delay_ms(100);

    // Isolate the pin 2 end of the diode
    ///pinMode(LED_N_SIDE,INPUT);
    DDRA &= notmask;
    ///digitalWrite(LED_N_SIDE,LOW);  // turn off internal pull-up resistor
    PORTA &= notmask;

    // Count how long it takes the diode to bleed back down to a logic zero
    for ( j = 0; j < 254; j++) 
    {
        if ( (PINA & mask) == 0) 
        {
            return j;
        }
        _delay_ms(2);
    }


    return 255;
}


int main()
{
    unsigned int value;

    DDRA |= 1<<PA2;     // Set data pin to output
    DDRA |= 1<<PA4;    // Set data pin to output


    while (1 == 1)
    {
        value = read_LED();

        bargraph(8-(value/32));
        //_delay_ms(5000);
        

    }
}
