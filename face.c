#include "face.h"
#include "RTC.h"

#define CONTROL_PORT    PORTA
#define CONTROL_DDR     DDRA
#define DATA_PIN        PA2
#define CLOCK_PIN       PA3

#define MIN_DATA        PA2
#define MIN_CLOCK       PA3
#define HOUR_DATA       PA1
#define HOUR_CLOCK      PA2  // Why did I do this?  I don't know.  It made sense at the time.

void WF_init(void)
{
    WF_clear();
}



void WF_clear(void)
{
    uint8_t i;

    for( i=0 ; i<12 ; i++)
    {
        CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
        // might need delays here.
        CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
    }

    for( i=0 ; i<12 ; i++)
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
    uint8_t LED;

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





