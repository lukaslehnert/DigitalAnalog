#include "face.h"

#include "RTC.h"
#include "LEDstatus.h"

#define CONTROL_PORT    PORTA
#define CONTROL_DDR     DDRA

#define MIN_DATA        PA2
#define MIN_CLOCK       PA3
#define HOUR_DATA       PA1
#define HOUR_CLOCK      PA2  // Why did I do this?  I don't know.  It made sense at the time.

void WF_init(void)
{
    CONTROL_DDR |= 1<<MIN_DATA;     // Set data pin to output
    CONTROL_DDR |= 1<<HOUR_DATA;    // Set data pin to output
    CONTROL_DDR |= 1<<MIN_CLOCK;    // Set data pin to output
    CONTROL_DDR |= 1<<HOUR_CLOCK;    // Set data pin to output

    WF_clear();
}



void WF_clear(void)
{
    uint8_t i;
    CONTROL_PORT &= ~(1<<MIN_DATA);
    CONTROL_PORT &= ~(1<<HOUR_DATA);

    for( i=0 ; i<16 ; i++)
    {
        CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
        _delay_ms(1);
        CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
    }

    for( i=0 ; i<16 ; i++)
    {
        CONTROL_PORT |= 1<<HOUR_CLOCK;      // Hour clock pin low
        _delay_ms(1);
        CONTROL_PORT &= ~(1<<HOUR_CLOCK);   // Hour clock pin high
    }

}


void WF_allOn(void)
{
    uint8_t i;
    CONTROL_PORT |= 1<<MIN_DATA;
    CONTROL_PORT |= 1<<HOUR_DATA;

    for( i=0 ; i<16 ; i++)
    {
        CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
        // might need delays here.
        CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
    }
    CONTROL_PORT |= 1<<HOUR_DATA;

    for( i=0 ; i<16 ; i++)
    {
        CONTROL_PORT &= ~(1<<HOUR_CLOCK);   // Hour clock pin high
        // might need delays here.
        CONTROL_PORT |= 1<<HOUR_CLOCK;      // Hour clock pin low
    }

}



void WF_displayTime(DateTime time)
{
    //time.minute
    //time.hour

    uint8_t i;

    // Start by clearing the watch face:
    WF_clear();

    // We have to set the minutes first, then the hours.
    CONTROL_PORT |= 1<<MIN_DATA;        // Minute data pin high
    CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
    // might need delays here.
    CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
    CONTROL_PORT &= ~(1<<MIN_DATA);     // Minute data pin low.  Note that this also shifts the hours.
    for( i = 0; i< time.minute/5 ; i++) // Now that we've loaded a bit into the SR, shift it over to
    {                                   // where we want it.
        CONTROL_PORT &= ~(1<<MIN_CLOCK);
        // might need delays here.
        CONTROL_PORT |= 1<<MIN_CLOCK;
    }


    // Now set the hours:
    CONTROL_PORT |= 1<<HOUR_DATA;       // Hour data pin high
    CONTROL_PORT &= ~(1<<HOUR_CLOCK);   // Hour clock pin high
    // might need delays here.
    CONTROL_PORT |= 1<<HOUR_CLOCK;      // Hour clock pin low
    CONTROL_PORT &= ~(1<<HOUR_DATA);    // Hour data pin low. 
    for( i = 0; i< time.hour ; i++)     // Now that we've loaded a bit into the SR, shift it over to
    {                                   // where we want it.
        CONTROL_PORT &= ~(1<<HOUR_CLOCK);
        // might need delays here.
        CONTROL_PORT |= 1<<HOUR_CLOCK;
    }



}



void WF_hourInc()
{
    CONTROL_PORT &= ~(1<<HOUR_CLOCK);
    // might need delays here.
    CONTROL_PORT |= 1<<HOUR_CLOCK;
}


void WF_freeRun()
{
    uint8_t minute = 1;
    for(;;)
    {

        if ( minute == 13 )
            minute = 1;

        if (minute == 1)
        {
            LEDflashSignal();
            // Set the new minute bit
            CONTROL_PORT |= 1<<MIN_DATA;
            _delay_ms(1);
            CONTROL_PORT &= ~(1<<MIN_CLOCK);
            _delay_ms(1);        // might need delays here.
            CONTROL_PORT |= 1<<MIN_CLOCK;
            //CONTROL_PORT &= ~(1<<MIN_DATA);
        }
        else
        {
            CONTROL_PORT &= ~(1<<MIN_DATA);
            _delay_ms(1);
            CONTROL_PORT &= ~(1<<MIN_CLOCK);
            _delay_ms(1);
            CONTROL_PORT |= 1<<MIN_CLOCK;
        }

        minute++;
        _delay_ms(1000);
        LEDflashSignal();
    }
}
