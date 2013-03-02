#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "LEDsensor.h"
#include "face.h"

# define nop()  __asm__ __volatile__ ("nop" ::)

#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.
volatile DateTime Time;
volatile uint8_t update_needed;
uint8_t data;

ISR(PCINT1_vect)
{
    cli();

    data = PINB & 0b00000011;
    switch (data)
    {
        case 0b00000001:  // PCINT9 is low (pressed) and PCINT8 is high (unpressed)
            update_needed = 1;
            while(data ^ 0b00000010) 
            {   // Spin until the button is released and the pin goes high.
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
                data = PINB & 0b00000010;
            }    
        break;
        case 0b00000010: // if only PCINT8 (RW) is pressed (meaning PCINT8 is low and PCINT9 is high)
            update_needed = 1;
            while(data ^ 0b00000001)
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
                data = PINB & 0b00000001;
            }
        break;
    }

nop();

}


ISR(PCINT0_vect)        // Interrupt Service Routine (called when PCINT0 changes state)
{   
    cli();      // disable interrupts while we handle the one that just happened
//    LEDon();


    data = PINA & 0b00000010;

    // Which pin caused the interrupt?
    switch (data)
    {
        case 0b000000010:  // PCINT1 (MFP)

            // MFP toggled.  Wake up and update the time.

            //Time = RTC_GetTime();
            //WF_displayTime(Time);
            // Handle any 1Hz tasks here
            break;
    }

nop();

} 



int main(void)
{
    uint8_t light_level = 0;
    uint8_t sensor_values[5];
    unsigned long int value;

    uint8_t i,j;


    // Setup interrupts
    PORTA |= (1<<PCINT1);  // Configure as input pin for RTC MFP
    PORTB |= (1<<PCINT8);  // Configure as input pin for RTC MFP
    DDRB |= (1<<PCINT8);    // Activate pullup
    PORTB |= (1<<PCINT9);  // Configure as input pin for RTC MFP
    DDRB |= (1<<PCINT9);    // Activate pullup
    PCMSK0 |= (1<<PCINT1);   // Pin Change interrupt Mask: listen to porta bit 1
    PCMSK1 |= (1<<PCINT8);   // Pin Change interrupt Mask: listen to portb bit 0
    PCMSK1 |= (1<<PCINT9);   // Pin Change interrupt Mask: listen to portb bit 1
    GIMSK |= (1<<PCIE0);   // enable PCINT interrupt in the General Interrupt Mask
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 


    for (i=0; i<5; i++)
        sensor_values[i] = 254;

    // We only need these if there is no time currently stored in the RTC.  So, never.
    // temptime = RTC_convert(__TIME__);
    //temptime = RTC_convert("11:56:00");
    //RTC_UpdateTime(temptime);


    i2c_init();
    RTC_init();
    WF_init();
//    WF_enable();
    //   WF_flashy();

    Time = RTC_GetTime();
    WF_displayTime(Time);



    for(;;)
    {
        cli();      // disable interrupts while we do our house keeping
        //    WF_clear();
        //        Time = RTC_GetTime();
        //      WF_displayTime(Time);

        light_level = LLS_read_LED();
        sensor_values[i%5] = light_level;
        i++;


        // Get an average light level:
        value = 0;
        for (j=0; j<5; j++)
            value += sensor_values[j];
        value = value / 5;
        if (value > 254)
            value = 254;
        PWM_DUTY_CYCLE = value;


        if(update_needed)
        {
            RTC_UpdateTime(Time);
            update_needed = 0;
        }

        Time = RTC_GetTime();
        WF_displayTime(Time);


        sei();         // enable all interrupts 
        nop();
//        LEDoff();
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
        nop();
    }
}

