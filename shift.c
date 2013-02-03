#include "shift.h"

#define CONTROL_PORT    PORTA
#define CONTROL_DDR     DDRA
#define DATA_PIN        PA2
#define CLOCK_PIN       PA4

uint8_t clock_noop = 0;



void SR_bargraph(int value)
{
    int i;
    for (i=8; i>0; i--)
        SR_push0(PA2, PA4);

    for (value=value; value>0; value--)
        SR_push1(PA2, PA4);
}



void SR_bytegraph(int value)
{
    int i;

    for (i=8; i>0; i--)
    {
        if ( (0b10000000 & value) == 0b10000000)
            SR_push1(PA2, PA4);
        else
            SR_push0(PA2, PA4);
        value = value << 1;
    }
}



void SR_init(uint8_t DATA, uint8_t CLOCK)
{
    CONTROL_DDR |= 1<<DATA_PIN;     // Set data pin to output
    CONTROL_DDR |= 1<<CLOCK_PIN;    // Set data pin to output

    SR_clear(DATA, CLOCK);      // Start with a clean slate
}



void SR_push1(uint8_t DATA, uint8_t CLOCK)
{
    CONTROL_PORT |= 1<<DATA;  // Data pin high

    SR_tick(CLOCK);
}



void SR_push0(uint8_t DATA, uint8_t CLOCK)
{
    CONTROL_PORT &= ~(1<<DATA); // Data pin low

    SR_tick(CLOCK);
}



void SR_tick(uint8_t CLOCK)
{
    CONTROL_PORT &= ~(1<<CLOCK); // Clock pin high
    //    clock_noop << 1;
    CONTROL_PORT |= 1<<CLOCK;  // Clock pin low
}



void SR_clear(uint8_t DATA, uint8_t CLOCK)
{
    uint8_t i;    

    for(i = 12 ; i>0 ; i--)
        SR_push0(DATA, CLOCK);
}


void SR_flashy(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;

    for(i=80 ; i>0 ; i=i*100/110)
    {
        SR_push1(PA1, PA2);
        SR_push1(PA2, PA3);
        for( j=0 ; j<12 ; j++ )
        {
            for ( k=0 ; k<=i ; k++) 
            {
                _delay_ms(1);
            }
            SR_push0(PA1, PA2);
            SR_push0(PA2, PA3);
        }
    }   
    for(i=100 ; i > 0 ; i--)
    {
        for(k=1 ; k > 0 ; k--)
        {
            SR_push1(PA2, PA3);
            SR_push1(PA1, PA2);
            for( j=0 ; j<12 ; j++ )
            {
                _delay_ms(1);
                SR_push0(PA2, PA3);
                _delay_ms(1);
                SR_push0(PA1, PA2);
            }
        }
        _delay_ms(1);
    }
    for(i=2 ; i<120 ; i=(i*3)/2)
    {
        SR_push1(PA1, PA2);
        SR_push1(PA2, PA3);
        for( j=0 ; j<12 ; j++ )
        {
            for ( k=0 ; k<=i ; k++)
            {
                _delay_ms(1);
            }
            SR_push0(PA1, PA2);
            SR_push0(PA2, PA3);
        }
    }

    SR_clear(PA1, PA2);
    SR_clear(PA2, PA3);
}

