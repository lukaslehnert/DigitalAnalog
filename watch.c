#define F_CPU 1000000UL
//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "USI_TWI_Master.h"
#include "watch.h"

void delayms(uint16_t millis) {
    //uint16_t loop;
    while ( millis ) {
        _delay_ms(1);
        millis--;
    }
}

#define SLAVE_ADDR  0x6F
#define MESSAGEBUF_SIZE       4

#define TWI_CMD_MASTER_WRITE 0x00
#define TWI_CMD_MASTER_READ  0x00

#define SEND_DATA             0x01
#define REQUEST_DATA          0x02
#define READ_DATA_FROM_BUFFER 0x03


int main(void) {
    //  DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    // while(1) {
    //   PORTB &= ~(1<<PB0); /* LED on */
    //   delayms(100);
    //   PORTB |= 1<<PB0; /* LED off */
    //   delayms(900);
    // }
    //

    unsigned char messageBuf[MESSAGEBUF_SIZE];
    unsigned char temp;


    USI_TWI_Master_Initialise();

    // Change this to the correct address, then find a better way to handle it (maybe a #define?)

    // send data to device
    messageBuf[0] = (SLAVE_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); 
    // The first byte must always consit of General Call code or the TWI slave address.
    messageBuf[1] = TWI_CMD_MASTER_WRITE;     // The first byte is used for commands.
    messageBuf[2] = 6;                // The second byte is used for the data.
    temp = USI_TWI_Start_Transceiver_With_Data( messageBuf, 3 );
    // ************ temp is now 0x00.  What does that mean?  TODO: Find out!
    // ************ "The slave did not acknowledge  the address"

    if(!temp)
        LEDflashData(USI_TWI_Get_State_Info());
    else
    {
        LEDflashSignal();
        LEDflashSignal();
    }

    // receive data from device

    messageBuf[0] = (SLAVE_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
    // The first byte must always consit of General Call code or the TWI slave address.
    messageBuf[1] = TWI_CMD_MASTER_READ;             // The first byte is used for commands.
    temp = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2 );

    // One might need to delay before proceding to enable the slave to interpret the last message,
    // and prepare the next request. Or as in this simple sample rerun the request until he responds. 
    //
    // Transmit request and get the received data from the transceiver buffer
    messageBuf[0] = (SLAVE_ADDR<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT); 
    // The first byte must always consit of General Call code or the TWI slave address.
    do
    {
        temp = USI_TWI_Start_Transceiver_With_Data( messageBuf, 2 );
    }
    while (USI_TWI_Get_State_Info() == USI_TWI_NO_ACK_ON_ADDRESS);

    PORTB = messageBuf[1];        // Store data on PORTB.

    if(temp)
        LEDflashData(temp);
    else
        LEDflashSignal();

    return 0;

}




void LEDon()
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB &= ~(1<<PB0); /* LED on */
}


void LEDflashSignal()
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB &= ~(1<<PB0); /* LED on */
    delayms(500);
    PORTB |= 1<<PB0; /* LED off */
    delayms(500);
}


void LEDflashData(unsigned char data)
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    unsigned char stored = data;
    unsigned char i;
    while(1)
    {
        LEDflashAlert();
        for(i = 8 ; i>0 ; i--) 
        {
            PORTB &= ~(1<<PB0); /* LED on */
            delayms(100);
            PORTB |= 1<<PB0; /* LED off */
            delayms(100);


            PORTB &= ~(1<<PB0); /* LED on */
            if((data & 0x80)) 
                delayms(500);
            else
                delayms(100);
            PORTB |= 1<<PB0; /* LED off */

            data <<= 1;

            delayms(900);
        }
        data = stored;
    }
}


void LEDflashAlert()
{
    DDRB |= 1<<PB0; // set PB0 to output.  "output" means "sink current"
    PORTB |= 1<<PB0; /* LED off */
    delayms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    delayms(100);
    PORTB |= 1<<PB0; /* LED off */
    delayms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    delayms(100);
    PORTB |= 1<<PB0; /* LED off */
    delayms(100);
    PORTB &= ~(1<<PB0); /* LED on */
    delayms(100);
    PORTB |= 1<<PB0; /* LED off */
    delayms(1000);
}
