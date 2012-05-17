#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"

#define I2CADDR 0xDE

#ifndef RTC
#define RTC

typedef struct {
        uint8_t second;
        uint8_t minute;
        uint8_t hour;

} DateTime;

uint8_t RTC_init (const char*);
uint8_t RTC_UpdateTime (DateTime);
DateTime RTC_GetTime ();

#endif
