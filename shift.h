
#include <avr/io.h>
#include <util/delay.h>


void SR_init(void);
void SR_outputByte(uint8_t);
void SR_push0(void);  // Push a 0 to the SR
void SR_push1(void);  // Push a 1 to the SR
void SR_tick(void);
void SR_clear(void);
