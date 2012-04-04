#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "shift.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.

ISR(PCINT1_vect)        // interrupt service routine 
{              // called when PCINT0 changes state 
    counter++;

    return; 
} 



int main(void) {

    DateTime Time;

    RTC_init(__TIME__);
    i2c_init();
    SR_init();

    Time = RTC_GetTime();
    // SR_outputByte(Time.minute);
    counter = Time.second;

    PORTB |= (1<<PCINT10);  // Configure as input pin
    PCMSK1 |= (1<<PCINT10);   // Pin Change interrupt Mask: listen to portb bit 2 
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 



    //unsigned char ret;

    for(;;)
    {
        cli();      // disable interrupts
     
        if(counter >=10)
        {
            counter = 0;
            Time = RTC_GetTime();
            delayms(1000);
        }

        if(!(counter % 4))
        {
            SR_outputByte(Time.minute);
            _delay_ms(30);
            SR_clear();
        }


        sei();         // enable all interrupts 
        // SLEEP
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
        // Woke up!


    }


}

