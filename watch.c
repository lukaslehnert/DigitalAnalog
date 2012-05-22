#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "face.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.

ISR(PCINT1_vect)        // Interrupt Service Routine (called when PCINT0 changes state)
{   

    // Which pin caused the interrupt?
    if (PINB & (0x01<<PCINT10))  // PCINT10 (Pin 5)(square wave input) is high
        counter++;

    LEDflashSignal();

    return; 
} 



int main(void) {

    DateTime Time;
    uint8_t lastcount = 0;

    i2c_init();
    RTC_init(__TIME__);
    WF_init();
//    WF_flashy();
    //Time.minute=42;
    //Time.hour=5;
    //WF_displayTime(Time);

/*    for(;;)
    {
        Time = RTC_GetTime();
        WF_displayTime(Time);
        _delay_ms(1000);
    }
*/
    counter = Time.second;

    // Setup interrupt
    PORTB |= (1<<PCINT10);  // Configure as input pin
    PCMSK1 |= (1<<PCINT10);   // Pin Change interrupt Mask: listen to portb bit 2 
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 



    //unsigned char ret;

    for(;;)
    {
        cli();      // disable interrupts while we do our house keeping

        Time = RTC_GetTime();
        WF_displayTime(Time);
        _delay_ms(500);

        if (counter > lastcount+1)
        {
            LEDflashSignal();

            WF_displayTime(Time);
        //    SR_outputByte(Time.minute);
        //    _delay_ms(75);
        //    SR_clear();
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
    //    sleep_mode();
        // Woke up!
    }
}

