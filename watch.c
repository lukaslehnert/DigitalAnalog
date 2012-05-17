#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "shift.h"
#include "face.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.

ISR(PCINT1_vect)        // Interrupt Service Routine (called when PCINT0 changes state)
{   

    // Which pin caused the interrupt?
    if (PINB & (0x01<<PCINT10))  // PCINT10 (Pin 5)(square wave input) is high
        counter++;

    return; 
} 



int main(void) {

    LEDflashSignal();
    LEDflashSignal();


    DateTime Time;
    uint8_t lastcount = 0;

//    RTC_init(__TIME__);
//    i2c_init();
    //WF_init();
    SR_init();
    SR_flashy();
    //WF_clear();
    Time.minute=42;
    Time.hour=5;
    //WF_displayTime(Time);

//    Time = RTC_GetTime();
//    SR_outputByte(Time.minute);
    counter = Time.second;
    
    WF_freeRun();

    for(;;)
    {
    }


    PORTB |= (1<<PCINT10);  // Configure as input pin
    PCMSK1 |= (1<<PCINT10);   // Pin Change interrupt Mask: listen to portb bit 2 
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 



    //unsigned char ret;

    for(;;)
    {
        cli();      // disable interrupts while we do our house keeping

        if (counter > lastcount+1)
        {
            SR_outputByte(Time.minute);
            _delay_ms(75);
            SR_clear();
            lastcount = counter;
        }

        if (counter >= 59) // There are 60 seconds in a minute, numbered 0-59.
        {
            Time = RTC_GetTime();
            counter = Time.second;
            lastcount = 0;
        }

        sei();         // enable all interrupts 

        // SLEEP
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
        // Woke up!
    }
}

