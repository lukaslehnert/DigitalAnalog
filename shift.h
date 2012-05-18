
#include <avr/io.h>
#include <util/delay.h>


void SR_init(uint8_t DATA, uint8_t CLOCK);
void SR_push0(uint8_t, uint8_t);  // Push a 0 to the SR
void SR_push1(uint8_t, uint8_t);  // Push a 1 to the SR
void SR_tick(uint8_t);
void SR_clear(uint8_t, uint8_t);
void SR_flashy(void);
