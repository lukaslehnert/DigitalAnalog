
#include <avr/io.h>
#include <util/delay.h>

#include "RTC.h"

void WF_init(void);
void WF_clear(void);
void WF_allOn(void);
void WF_freeRun(void);
void WF_displayTime(DateTime);
void WF_tickMinute(void);
void WF_tickHour(void);
void WF_hourInc(void);
