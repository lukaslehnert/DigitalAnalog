#include "shift.h"

#define CONTROL_PORT    PORTA
#define CONTROL_DDR     DDRA
#define DATA_PIN        PA2
#define CLOCK_PIN       PA3



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
    _delay_ms(100);
    CONTROL_PORT &= ~(1<<CLOCK_PIN); // Clock pin high
    _delay_ms(100);
    CONTROL_PORT |= 1<<CLOCK_PIN;  // Clock pin low
}



void SR_clear(void)
{
    uint8_t i;    

    for(i = 8 ; i>0 ; i--)
        SR_push0();
}


