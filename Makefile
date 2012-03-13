CC=/usr/bin/avr-g++
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny24a -D__AVR_ATtiny24A__
OBJ2HEX=/usr/bin/avr-objcopy
PROG=/usr/bin/avrdude
INCLUDES=-I. -I/usr/avr/include

MCU=attiny24
TARGET=watch
objects = wire.o twi.o 

$(TARGET) : $(TARGET).hex

$(TARGET).hex : $(TARGET).c $(objects)

%.o : %.cpp
	@echo building .o
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.o : %.c
	@echo building .o
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.obj : %.o
	@echo building .obj
	$(CC) $(CFLAGS) $< $(objects) -o $@

%.hex : %.obj
	@echo building .hex
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

.PHONY : clean flash

flash : $(TARGET).hex
	$(PROG) -p $(MCU) -P /dev/ttyS0 -c usbasp -U flash:w:$(TARGET).hex

clean :
	rm -f *.hex *.obj *.o

