#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "face.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.
volatile DateTime Time;

ISR(PCINT1_vect)        // Interrupt Service Routine (called when PCINT0 changes state)
{   
    cli();      // disable interrupts while we handle the one that just happened
    LEDon();

    Time = RTC_GetTime();


    // Which pin caused the interrupt?
    switch (PINB)
    {
        case 0b000000111:  // PCINT10 (MFP)
            WF_displayTime(Time);
            // Handle any 1Hz tasks here
            break;
        case 0b00000001: // if only PCINT9 (FF) is pressed (meaning PCINT8 is low and PCINT9 is high)
        case 0b00000101: // Also trigger if the MFP is high
            while((PINB & 0b00000011) ^ 0b00000011)
            {   // Spin until the buttons are released.
                Time.minute++;
                if(Time.minute > 59)
                {
                    Time.minute = 0;
                    Time.hour++;
                    if (Time.hour > 11)
                        Time.hour=0;
                }
                WF_displayTime(Time);
                _delay_ms(100);
            }    
            RTC_UpdateTime(Time);
            break;
        case 0b00000010: // if only PCINT8 (RW) is pressed (meaning PCINT9 is low and PCINT8 is high)
        case 0b00000110: // Also trigger if the MFP is high
            while((PINB & 0b00000011) ^ 0b00000011)
            {   // Spin until the buttons are released.
                if(Time.minute == 0)
                {   // Don't decrement if we're at 0, or we'll wrap around
                    Time.minute = 59;
                    if(Time.hour == 0) // Don't decrement if we're at 0, or we'll wrap around
                        Time.hour = 11;
                    else
                        Time.hour--;
                }
                else
                    Time.minute--;

                WF_displayTime(Time);
                _delay_ms(100);
            }
            RTC_UpdateTime(Time);
            break;
    }




    return; 
} 



int main(void) {

    uint8_t lastcount = 0;


    i2c_init();
    RTC_init();
    WF_init();
    //    WF_flashy();  // Add this back in if we have the space to spare.


    // We only need these if there is no time currently stored in the RTC.  So, never.
    // DateTime temptime;
    // temptime = RTC_convert(__TIME__);
    // temptime = RTC_convert("12:10:00");
    // RTC_UpdateTime(temptime);

    Time = RTC_GetTime();
    WF_displayTime(Time);

    // Setup interrupts
    PORTB |= (1<<PCINT10);  // Configure as input pin for RTC MFP
    //    PORTB |= (1<<PCINT9);  // Configure as input pin for RW button
    //    PORTB |= (1<<PCINT8);  // Configure as input pin for FF button
    PCMSK1 |= (1<<PCINT10);   // Pin Change interrupt Mask: listen to portb bit 2 
    PCMSK1 |= (1<<PCINT9);   // Pin Change interrupt Mask: listen to portb bit
    PCMSK1 |= (1<<PCINT8);   // Pin Change interrupt Mask: listen to portb bit
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 



    for(;;)
    {
        cli();      // disable interrupts while we do our house keeping

        LEDoff();
        // SLEEP
        sei();         // enable all interrupts 
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
        // Woke up!
    }
}

