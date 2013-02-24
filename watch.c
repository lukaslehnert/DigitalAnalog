#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "LEDsensor.h"
#include "face.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.
volatile DateTime Time;


ISR(PCINT1_vect)
{
    cli();

    uint8_t data;

    data = PINB & 0b00000011;

    switch (data)
    {

        Time = RTC_GetTime();
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
        Time = RTC_GetTime();
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

    sei();

    return;
}


ISR(PCINT0_vect)        // Interrupt Service Routine (called when PCINT0 changes state)
{   
    cli();      // disable interrupts while we handle the one that just happened
    LEDon();

    uint8_t data;

    data = PINA & 0b00000010;

    // Which pin caused the interrupt?
    switch (data)
    {
        case 0b000000010:  // PCINT1 (MFP)

            Time = RTC_GetTime();
            WF_displayTime(Time);
            // Handle any 1Hz tasks here
            break;
    }

    sei();

    return; 
} 



int main(void)
{
    uint8_t light_level = 0;
    uint8_t sensor_values[5];
    unsigned long int value;

    uint8_t i,j;


    // Setup interrupts
    PORTA |= (1<<PCINT1);  // Configure as input pin for RTC MFP
    PCMSK0 |= (1<<PCINT1);   // Pin Change interrupt Mask: listen to porta bit 1
    //PCMSK1 |= (1<<PCINT9);   // Pin Change interrupt Mask: listen to portb bit
    //PCMSK1 |= (1<<PCINT8);   // Pin Change interrupt Mask: listen to portb bit
    GIMSK |= (1<<PCIE0);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 


    for (i=0; i<5; i++)
        sensor_values[i] = 254;

    // We only need these if there is no time currently stored in the RTC.  So, never.
    DateTime temptime;
    //    temptime = RTC_convert(__TIME__);
    temptime = RTC_convert("12:45:00");
    RTC_UpdateTime(temptime);


    i2c_init();
    RTC_init();
    WF_init();
//    WF_enable();
    //   WF_flashy();

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


        sei();         // enable all interrupts 
        LEDoff();
        //set_sleep_mode(SLEEP_MODE_IDLE);
        //sleep_mode();
    }
}

