CC=/usr/bin/avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny24
OBJ2HEX=/usr/bin/avr-objcopy
PROG=/usr/bin/avrdude
MCU=attiny24
TARGET=watch

objects = watch.o

$(TARGET) : $(TARGET).hex
	
	

%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

.PHONY : clean flash

flash : $(TARGET).hex
	$(PROG) -p $(MCU) -P /dev/ttyS0 -c usbasp -U flash:w:$(TARGET).hex

clean :
	rm -f *.hex *.obj *.o

