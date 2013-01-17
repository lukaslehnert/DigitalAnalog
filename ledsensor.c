
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


#define bit_get(p,m) ((p) & (m)) 
#define BIT(x) (0x01 << (x)) 

#define POS PA0
#define NEG PA1


int read_LED ()
{
    unsigned int j;
    unsigned int bit;

    //Apply reverse voltage, charge up the pin and led capacitance
    ///pinMode(LED_N_SIDE,OUTPUT);
    DDRA |= 1<<NEG;
    ///pinMode(LED_P_SIDE,OUTPUT);
    DDRA |= 1<<POS;
    ///digitalWrite(LED_N_SIDE,HIGH);
    PORTA |= 1<<NEG;
    ///digitalWrite(LED_P_SIDE,LOW);
    PORTA &= ~(1<<POS);

    _delay_ms(500);

    // Isolate the pin 2 end of the diode
    ///pinMode(LED_N_SIDE,INPUT);
    DDRA &= ~(1<<NEG);
    ///digitalWrite(LED_N_SIDE,LOW);  // turn off internal pull-up resistor
    PORTA &= ~(1<<NEG);

    // Count how long it takes the diode to bleed back down to a logic zero
    for ( j = 0; j < 30000; j++) 
    {
        bit = bit_get(PORTA, BIT(NEG));
        if ( ! bit ) 
            return j;
    }




}

int main()
{
    unsigned int value;

    while (1)
    {
        

        value = read_LED();

    }
}
