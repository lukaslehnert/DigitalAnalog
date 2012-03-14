F_CPU = 8000000
DEVICE = attiny24a

CFLAGS = -g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -DF_CPU=$(F_CPU)UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS = -fno-exceptions -Wundef

CC=avr-gcc
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -Wl,-relax

TARGET=I2C
OBJECT_FILES=I2C.o

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
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

AVRDUDE_DEVICE = atmega1284p
PORT = /dev/ttyACM0
AVRDUDE=avrdude

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex
