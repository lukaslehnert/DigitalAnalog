#include <avr/io.h>
#include <util/delay.h>



void LEDon();
void LEDoff();
void LEDflashSignal();
void LEDflashData(unsigned char);
void LEDflashAlert();

void delayms(uint16_t);

