
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


/* Comment courtesy of Arduino project.  
 * I've used their code as a basis for my own,
 * porting to to pure AVR C in the process. */


#include "LEDsensor.h"

# define nop()  __asm__ __volatile__ ("nop" ::) 


int LLS_read_LED ()
{
    unsigned int j;
    unsigned int mask, notmask;
    unsigned int i, delay;

    delay = 160;


    mask = 1<<NEG;
    notmask = ~(1<<NEG);


    //Apply reverse voltage, charge up the pin and led capacitance
    DDRA |= 1<<NEG;
    DDRA |= 1<<POS;
    PORTA |= 1<<NEG;
    PORTA &= ~(1<<POS);
    _delay_ms(50);

    // Isolate the pin 2 end of the diode
    DDRA &= notmask;
    PORTA &= notmask;

    // Count how long it takes the diode to bleed back down to a logic zero
    for ( j = 0; j < 254; j++) 
    {
        if ( (PINA & mask) == 0) 
            return j;

        //_delay_ms(1);  // 1 Millisecond is much to long.  Lets cound clock cycles instead:
        for (i=0; i<delay; i++)
            nop();
        
    }

    return 255;
}


// suggested usage:

/*
int main()
{
    unsigned int i, j;
    unsigned int values[5];
    unsigned long int value;


    DDRA |= 1<<PA2;     // Set data pin to output
    DDRA |= 1<<PA4;    // Set data pin to output

    
    for (i=0; i<5; i++)
        values[i] = 254;


    while (1 == 1)
    {
        values[i%5] = LLS_read_LED();
        i++;
            
        value = 0;
        for (j=0; j<5; j++)
            value += values[j];
        value = value / 5;

        SR_bargraph(8-(value/32));
        

    }
}

*/

