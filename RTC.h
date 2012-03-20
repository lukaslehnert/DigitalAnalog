

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"

typedef struct {
//        DateTime DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour , uint8_t min , uint8_t sec );
        uint8_t hour, minute, second;

} DateTime;

uint8_t RTC_SetTime (const char*);
