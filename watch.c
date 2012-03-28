#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"
#include "RTC.h"
#include "LEDstatus.h"
#include "shift.h"


#define I2CADDR 0xDE

volatile uint8_t counter;   // Declare volatile to prevent GCC from optimizing it out.

ISR(PCINT1_vect)        // interrupt service routine 
{              // called when PCINT0 changes state 
    counter++;

    return; 
} 



int main(void) {

    DateTime Time;

    RTC_init(__TIME__);
    i2c_init();
    SR_init();

    Time = RTC_GetTime();
    // SR_outputByte(Time.minute);
    counter = Time.second;

    PORTB |= (1<<PCINT10);  // Configure as input pin
    PCMSK1 |= (1<<PCINT10);   // Pin Change interrupt Mask: listen to portb bit 2 
    GIMSK |= (1<<PCIE1);   // enable PCINT interrupt in the General Interrupt Mask
    sei();         // enable all interrupts 



    //unsigned char ret;

    for(;;)
    {

        cli();      // disable interrupts
        if(counter >= 100)
        {
            counter = 0;
            Time = RTC_GetTime();
            delayms(1000);
        }

        if(!(counter % 4))
        {
            SR_outputByte(Time.minute);
            _delay_ms(30);
            SR_clear();
        }


        sei();         // enable all interrupts 
        // SLEEP
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
        // Woke up!


    }

    /*ret = i2c_start(I2CADDR+I2C_WRITE);       // set device address and write mode

      if ( ret )
      {
    // failed to issue start condition, possibly no device found 
    i2c_stop();
    LEDflashAlert();    // Flash an alert signal to indicate that we have an invalid start condition.
    LEDflashAlert();                        
    }
    else
    {
    // issuing start condition ok, device accessible 
    i2c_write(0x00);                         // write address = 0
    i2c_rep_start(I2CADDR+I2C_READ);        // set device address and read mode
    ret = i2c_readNak();                       // read one byte form address 0
    i2c_stop();                              // set stop condition = release bus


    i2c_start_wait(I2CADDR+I2C_WRITE);       // set device address and write mode
    i2c_write(0x00);                       // write address = 0
    i2c_write(0b01010101);                       // ret=0 -> Ok, ret=1 -> no ACK 
    i2c_stop();                            // set stop conditon = release bus

    // write ok, read value back from eeprom address 0x05, wait until 
    //            the device is no longer busy from the previous write operation 
    i2c_start_wait(I2CADDR+I2C_WRITE);     // set device address and write mode
    i2c_write(0x00);                        // write address = 0
    i2c_rep_start(I2CADDR+I2C_READ);       // set device address and read mode
    ret = i2c_readNak();                    // read one byte
    i2c_stop();

    LEDflashData(ret);

    // write 0x70,0x71,072,073 to eeprom address 0x00..0x03 (Page Write),
    //            wait until the device is no longer busy from the previous write operation 
    //        i2c_start_wait(I2CADDR+I2C_WRITE);     // set device address and write mode
    //       i2c_write(0x00);                        // write start address = 0
    //       i2c_write(0x70);                        // write data to address 0
    //       i2c_write(0x71);                        //    "    "   "    "    1
    //       i2c_write(0x72);                        //    "    "   "    "    2
    //       i2c_write(0x74);                        //    "    "   "    "    3
    //       i2c_stop();                             // set stop conditon = release bus

    // write ok, read value back from eeprom address 0..3 (Sequencial Read),
    //            wait until the device is no longer busy from the previous write operation 
    //      i2c_start_wait(I2CADDR+I2C_WRITE);      // set device address and write mode
    //      i2c_write(0x00);                         // write address = 0
    //      i2c_rep_start(I2CADDR+I2C_READ);        // set device address and read mode
    //     ret = i2c_readAck();                       // read one byte form address 0
    //    ret = i2c_readAck();                       //  "    "    "    "     "    1
    //   ret = i2c_readAck();                       //  "    "    "    "     "    2
    //  ret = i2c_readNak();                       //  "    "    "    "     "    3
    // i2c_stop();                              // set stop condition = release bus
    */

    return 0;

}

