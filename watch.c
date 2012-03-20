#define F_CPU 1000000UL
//#define __AVR_ATtiny24A__
#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"
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

    unsigned char ret;



#define I2CADDR 0xDE

    i2c_init();
    ret = i2c_start(I2CADDR+I2C_WRITE);       // set device address and write mode

    if ( ret )
    {
        /* failed to issue start condition, possibly no device found */
        i2c_stop();
        LEDflashAlert();    // Flash an alert signal to indicate that we have an invalid start condition.
        LEDflashAlert();                        
    }
    else
    {
        /* issuing start condition ok, device accessible */
        i2c_write(0x00);                         // write address = 0
        i2c_rep_start(I2CADDR+I2C_READ);        // set device address and read mode
        ret = i2c_readNak();                       // read one byte form address 0
        i2c_stop();                              // set stop condition = release bus

       
        i2c_start_wait(I2CADDR+I2C_WRITE);       // set device address and write mode
        i2c_write(0x00);                       // write address = 0
        i2c_write(0b01010101);                       // ret=0 -> Ok, ret=1 -> no ACK 
        i2c_stop();                            // set stop conditon = release bus

        /* write ok, read value back from eeprom address 0x05, wait until 
         *            the device is no longer busy from the previous write operation */
        i2c_start_wait(I2CADDR+I2C_WRITE);     // set device address and write mode
        i2c_write(0x00);                        // write address = 0
        i2c_rep_start(I2CADDR+I2C_READ);       // set device address and read mode
        ret = i2c_readNak();                    // read one byte
        i2c_stop();

        LEDflashData(ret);

       /* write 0x70,0x71,072,073 to eeprom address 0x00..0x03 (Page Write),
        *            wait until the device is no longer busy from the previous write operation */
//        i2c_start_wait(I2CADDR+I2C_WRITE);     // set device address and write mode
 //       i2c_write(0x00);                        // write start address = 0
 //       i2c_write(0x70);                        // write data to address 0
 //       i2c_write(0x71);                        //    "    "   "    "    1
 //       i2c_write(0x72);                        //    "    "   "    "    2
 //       i2c_write(0x74);                        //    "    "   "    "    3
 //       i2c_stop();                             // set stop conditon = release bus

        /* write ok, read value back from eeprom address 0..3 (Sequencial Read),
         *            wait until the device is no longer busy from the previous write operation */
  //      i2c_start_wait(I2CADDR+I2C_WRITE);      // set device address and write mode
  //      i2c_write(0x00);                         // write address = 0
  //      i2c_rep_start(I2CADDR+I2C_READ);        // set device address and read mode
   //     ret = i2c_readAck();                       // read one byte form address 0
    //    ret = i2c_readAck();                       //  "    "    "    "     "    1
     //   ret = i2c_readAck();                       //  "    "    "    "     "    2
      //  ret = i2c_readNak();                       //  "    "    "    "     "    3
       // i2c_stop();                              // set stop condition = release bus

    }

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
