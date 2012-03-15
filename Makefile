DEVICE = attiny24a
F_CPU = 1000000

CFLAGS = -g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -DF_CPU=$(F_CPU)UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS = -fno-exceptions -Wundef

CC=avr-gcc
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -Wl,-relax

TARGET=watch
OBJECT_FILES=watch.o USI_TWI_Master.o

.SECONDARY : $(OBJECT_FILES)

all: $(TARGET).hex

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o *.hex *.obj

test1.o: test1.cc


%.hex: %.obj
	@echo building .hex
	$(OBJ2HEX) -R .eeprom -O ihex $< $@
	avr-size *.o *.obj *.hex

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

AVRDUDE_DEVICE = attiny24
PORT = /dev/ttyS0
AVRDUDE = avrdude
PROGRAMMER = usbasp

flash: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(TARGET).hex
