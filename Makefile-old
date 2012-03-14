CC=/usr/bin/avr-g++
CFLAGS=-mmcu=attiny24a -Os -Wall -mrelax -ffunction-sections -fdata-sections
#-D__AVR_ATtiny24A__ 
#-mcall-prologues
OBJ2HEX=/usr/bin/avr-objcopy
PROG=/usr/bin/avrdude
INCLUDES=-I. -I/usr/avr/include

MCU=attiny24
TARGET=watch
objects = TinyWireM.o USI_TWI_Master.o


#$(TARGET) : $(TARGET).hex

$(TARGET).hex : $(TARGET).c $(objects) Makefile

%.o : %.cpp
	@echo building .o
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.o : %.c
	@echo building .o
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.obj : %.o
	@echo building .obj
	$(CC) $(CFLAGS) -Wl,-gc-sections $< $(objects) -o $@

%.hex : %.obj
	@echo building .hex
	$(OBJ2HEX) -R .eeprom -O ihex $< $@
	du -b *.o *.obj *.hex

.PHONY : clean flash

flash : $(TARGET).hex
	$(PROG) -p $(MCU) -P /dev/ttyS0 -c usbasp -U flash:w:$(TARGET).hex

clean :
	rm -f *.hex *.obj *.o

