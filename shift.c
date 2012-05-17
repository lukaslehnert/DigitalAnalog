#include "shift.h"

#define CONTROL_PORT    PORTA
#define CONTROL_DDR     DDRA
#define DATA_PIN        PA2
#define CLOCK_PIN       PA3

uint8_t clock_noop = 0;



void SR_init(void)
{
    CONTROL_DDR |= 1<<DATA_PIN;     // Set data pin to output
    CONTROL_DDR |= 1<<CLOCK_PIN;    // Set data pin to output

    SR_clear();      // Start with a clean slate
}


void SR_outputByte(uint8_t byte)
{

    uint8_t i;    

    for(i = 8 ; i>0 ; i--)
    {   
        if((byte & 0x80))
            SR_push1();
        else
            SR_push0();

        byte <<= 1;
    }
}


void SR_push1(void)
{
    CONTROL_PORT |= 1<<DATA_PIN;  // Data pin high

    SR_tick();
}



void SR_push0(void)
{
    CONTROL_PORT &= ~(1<<DATA_PIN); // Data pin low

    SR_tick();
}



void SR_tick(void)
{
    CONTROL_PORT &= ~(1<<CLOCK_PIN); // Clock pin high
    //    clock_noop << 1;
    CONTROL_PORT |= 1<<CLOCK_PIN;  // Clock pin low
}



void SR_clear(void)
{
    uint8_t i;    

    for(i = 12 ; i>0 ; i--)
        SR_push0();
}


void SR_flashy(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;

    for(i=80 ; i>0 ; i=i*100/110)
    {
        SR_push1();
        for( j=0 ; j<12 ; j++ )
        {
            for ( k=0 ; k<=i ; k++) 
            {
                _delay_ms(1);
            }
            SR_push0();
        }
    }   

    SR_clear();
}

