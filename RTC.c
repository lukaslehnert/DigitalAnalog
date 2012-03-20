#include <avr/pgmspace.h>
#include "RTC.h"

#define SLAVE_ADDR 0x6F

int i = 0; //The new wire library needs to take an int when you are sending for the zero register

const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 }; //has to be const or compiler compaints

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second


static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
uint8_t RTC_SetTime (const char* time) {
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




static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }


