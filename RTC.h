

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"

#define I2CADDR 0xDE


typedef struct {
        uint8_t second;
        uint8_t minute;
        uint8_t hour;

} DateTime;

uint8_t RTC_init (const char*);
uint8_t RTC_UpdateTime (DateTime);
DateTime RTC_GetTime ();


