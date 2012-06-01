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
        CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
    }

    for( i=0 ; i<16 ; i++)
    {
        CONTROL_PORT |= 1<<HOUR_CLOCK;      // Hour clock pin low
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

    //uint8_t fancyness = 40;
    uint8_t fancyness = 0;

    uint8_t i;
    uint8_t minticks;
    uint8_t hourticks;
    _Bool extralight = 0;
    _Bool wraparound = 0;

    switch (time.minute % 5)
    {
        case 0:
        case 1:
            extralight=0;
            minticks = (time.minute / 5) -1;
            break;
        case 2:
        case 3:
            extralight=1;
            minticks = (time.minute / 5) -1;
            if (time.minute == 3)
                wraparound = 1;
            break;
        case 4:
            extralight=0;
            minticks = (time.minute / 5);
            break;
    }

    if (minticks>11)
        minticks=11;

    switch(time.hour)
    {
        case 0:
            hourticks = 11;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
                hourticks = time.hour - 1;
                break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
                    hourticks = time.hour - 13;
                    break;
        default:
                        LEDflashAlert();
                        hourticks=11;
                        break;
    }


    // Start by clearing the watch face:
    WF_clear();

    // We have to set the minutes first, then the hours.
    for(i=0; i<1+extralight; i++) // shift in the correct number of lights
    {
        CONTROL_PORT |= 1<<MIN_DATA;        // Minute data pin high
        CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
        CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
        CONTROL_PORT &= ~(1<<MIN_DATA);     // Minute data pin low.  Note that this also shifts the hours.
    }

    for( i = 0; i < minticks-wraparound ; i++) // Now that we've loaded a bit into the SR, shift it over to
    {                                   // where we want it.
        CONTROL_PORT &= ~(1<<MIN_CLOCK);
        CONTROL_PORT &= ~(1<<HOUR_CLOCK);
        _delay_ms(fancyness);
        CONTROL_PORT |= 1<<MIN_CLOCK;
        CONTROL_PORT |= 1<<HOUR_CLOCK;
    }

    if(wraparound)  // Right at the top of the hour,
        {                                   // we have to shift the first LED all the way around, 
            // then add the second one
            CONTROL_PORT |= 1<<MIN_DATA;        // Minute data pin high
            CONTROL_PORT &= ~(1<<MIN_CLOCK);    // Minute clock pin high
            CONTROL_PORT |= 1<<MIN_CLOCK;       // Minute clock pin low
            CONTROL_PORT &= ~(1<<MIN_DATA);     // Minute data pin low.  Note that this also shifts the hours.
        }

    // Now set the hours:
    CONTROL_PORT |= 1<<HOUR_DATA;       // Hour data pin high
    CONTROL_PORT &= ~(1<<HOUR_CLOCK);   // Hour clock pin high
    CONTROL_PORT |= 1<<HOUR_CLOCK;      // Hour clock pin low
    CONTROL_PORT &= ~(1<<HOUR_DATA);    // Hour data pin low. 
    for( i = 0; i< hourticks ; i++)     // Now that we've loaded a bit into the SR, shift it over to
    {                                   // where we want it.
        CONTROL_PORT &= ~(1<<HOUR_CLOCK);
        _delay_ms(fancyness);
        CONTROL_PORT |= 1<<HOUR_CLOCK;
    }



}

/*

   void WF_freeRun()
   {
   WF_clear();

   uint8_t minute = 1;
   uint8_t hour = 1;

   for(;;)
   {

   if ( minute == 13 )
   {
   minute = 1;
   hour++;
   }

   if ( hour == 13 )
   hour = 1;

   if (minute == 1)
   {
   LEDflashSignal();
// Set the new minute bit
CONTROL_PORT |= 1<<MIN_DATA;
_delay_ms(2);
CONTROL_PORT &= ~(1<<MIN_CLOCK);
_delay_ms(2);        // might need delays here.
CONTROL_PORT |= 1<<MIN_CLOCK;
}
else
{
CONTROL_PORT &= ~(1<<MIN_DATA);
_delay_ms(2);
CONTROL_PORT &= ~(1<<MIN_CLOCK);
_delay_ms(2);
CONTROL_PORT |= 1<<MIN_CLOCK;
}

if (hour == 1)
{
CONTROL_PORT |= 1<<HOUR_DATA;
_delay_ms(2);
CONTROL_PORT &= ~(1<<HOUR_CLOCK);
_delay_ms(2);        // might need delays here.
CONTROL_PORT |= 1<<HOUR_CLOCK;
}
else
{
CONTROL_PORT &= ~(1<<HOUR_DATA);
_delay_ms(2);
CONTROL_PORT &= ~(1<<HOUR_CLOCK);
_delay_ms(2);
CONTROL_PORT |= 1<<HOUR_CLOCK;
}





minute++;
_delay_ms(1000);
LEDflashSignal();
}
}



void WF_tick(_Bool ZeroOrOne, uint8_t DATA, uint8_t CLOCK)
{
    if(ZeroOrOne)
        CONTROL_PORT |= 1<<DATA;
    else
        CONTROL_PORT &= ~(1<<DATA);

    CONTROL_PORT &= ~(1<<CLOCK);
    CONTROL_PORT |= 1<<CLOCK;
}

   void WF_flashy(void)
   {
   uint8_t i;
   uint8_t j;
   uint8_t k;

   for(i=80 ; i>0 ; i=i*100/110)
   {
   WF_tick(1, PA1, PA2);
   WF_tick(1, PA2, PA3);
   for( j=0 ; j<12 ; j++ )
   {
   for ( k=0 ; k<=i ; k++)
   {
   _delay_ms(1);
   }
   WF_tick(0, PA1, PA2);
   WF_tick(0, PA2, PA3);
   }
   }
   for(i=100 ; i > 0 ; i--)
   {
   for(k=1 ; k > 0 ; k--)
   {
   WF_tick(1, PA2, PA3);
   WF_tick(1, PA1, PA2);
   for( j=0 ; j<12 ; j++ )
   {
   _delay_ms(1);
   WF_tick(0, PA2, PA3);
   _delay_ms(1);
   WF_tick(0, PA1, PA2);
   }
   }
   _delay_ms(1);
   }
   for(i=2 ; i<120 ; i=(i*3)/2)
   {
   WF_tick(1, PA1, PA2);
   WF_tick(1, PA2, PA3);
   for( j=0 ; j<12 ; j++ )
   {
   for ( k=0 ; k<=i ; k++)
   {
   _delay_ms(1);
   }
   WF_tick(0, PA1, PA2);
   WF_tick(0, PA2, PA3);
   }
   }

   WF_clear();
   }
   */
