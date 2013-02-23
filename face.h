
#include <avr/io.h>
#include <util/delay.h>

#include "RTC.h"

void WF_init(void);
void WF_enable(void);
void WF_disable(void);
void WF_clear(void);
void WF_allOn(void);
void WF_freeRun(void);
void WF_displayTime(DateTime);
void WF_flashy(void);
void WF_tick(_Bool ZeroOrOne, uint8_t hourormin);
