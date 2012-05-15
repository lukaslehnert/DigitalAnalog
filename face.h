
#include <avr/io.h>
#include <util/delay.h>


void WF_init(void);
void WF_clear(void);
void WF_displayTime(DateTime);
void WF_tickMinute(void);
void WF_tickHour(void);
