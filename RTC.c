
#include <avr/pgmspace.h>
#include "RTC.h"

#include "LEDstatus.h"
#include "shift.h"

#define SLAVE_ADDR 0x6F



static uint8_t conv2d(const char* p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

// A convenient accessor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
uint8_t RTC_SetTime (const char* time) 
{
    // sample input: time = "12:34:56"
    DateTime temptime;
    uint8_t code = 0;

    temptime.hour = conv2d(time);
    temptime.minute = conv2d(time + 3);
    temptime.second = conv2d(time + 6);

    // Now write to RTC device.

    RTC_UpdateTime(temptime);

    // return 0 for success
    // return error code for failure

    return code;
}


// Push a DateTime back to the RTC device.
uint8_t RTC_UpdateTime (DateTime time) 
{

    unsigned char ret;

    // Now write to RTC device.
    ret = i2c_start(I2CADDR+I2C_WRITE);       // set device address and write mode

    if ( ret )
    {
        /* failed to issue start condition, possibly no device found */
        i2c_stop();
        SR_outputByte(ret);
        for(;;)
            LEDflashAlert();    // Flash an alert signal to indicate that we have an invalid start condition.
    }
    else
    {
        i2c_start_wait(I2CADDR+I2C_WRITE);     // set device address and write mode
        ret = i2c_write(0x00);                       // write address = 0
        ret += i2c_write(bin2bcd(time.second) & 0x80);// write data to address 0
        ret += i2c_write(bin2bcd(time.minute));       // write data to address 1
        ret += i2c_write(bin2bcd(time.hour) & 0x40);         // write data to address 2
        // The error code from below indicates that these writes are failing (0b00000011)
        i2c_stop();                            // set stop conditon = release bus
        if ( ret )
        {
            SR_outputByte(ret);
            for(;;)
                LEDflashAlert();    // Flash an alert signal to indicate that we have a failed write
        }
    }

    // return 0 for success
    // return error code for failure
    return ret;

}


//Retrieve a DateTime from the RTC device
DateTime RTC_GetTime ()
{
    unsigned char ret;
    DateTime temptime;

    i2c_init();
    ret = i2c_start(I2CADDR+I2C_WRITE);
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
        i2c_write(0x00);                            // write address = 0
        i2c_rep_start(I2CADDR+I2C_READ);            // set device address and read mode
        temptime.second = bcd2bin(i2c_readAck() & ~(0x80));   // read one byte form address 0
        temptime.minute = bcd2bin(i2c_readAck());   // read one byte form address 0
        temptime.hour = bcd2bin(i2c_readNak() & ~(0x40));     // read one byte form address 0
        i2c_stop();                                 // set stop condition = release bus
    }
    return temptime;
}



