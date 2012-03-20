#include <avr/pgmspace.h>
#include "RTC.h"

#define SLAVE_ADDR 0x6F



static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// A convenient accessor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
uint8_t RTC_SetTime (const char* time) 
{
    // sample input: time = "12:34:56"
    extern DateTime Time;
    uint8_t code = 0;

    Time.hour = conv2d(time);
    Time.minute = conv2d(time + 3);
    Time.second = conv2d(time + 6);
    
    // Now write to RTC device.
    

    // return 0 for success
    // return error code for failure

    return code;
}


// Push a DateTime back to the RTC device.
uint8_t RTC_UpdateTime (DateTime time) 
{

}


//Retrieve a DateTime from the RTC device
DateTime RTC_GetTime ()
{

}


static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }


