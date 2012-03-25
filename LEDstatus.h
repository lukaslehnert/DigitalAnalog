#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>



void LEDon();
void LEDoff();
void LEDflashSignal();
void LEDflashData(unsigned char);
void LEDflashAlert();

void delayms(uint16_t);

