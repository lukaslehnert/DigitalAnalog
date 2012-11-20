DEVICE = attiny24a
F_CPU = 8000000

CFLAGS = -g -Wall -mmcu=$(DEVICE) -Os -DF_CPU=$(F_CPU)UL -mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CFLAGS = -g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os -DF_CPU=$(F_CPU)UL -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS = $(CFLAGS) -fno-exceptions -Wundef

CC=avr-gcc
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -Wl,-relax

TARGET=watch
OBJECT_FILES=watch.o i2cmaster.o LEDstatus.o RTC.o face.o

.SECONDARY : $(OBJECT_FILES)

all: $(TARGET).hex

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.hex *.obj pcb/*.[bs]\#[0-9] pcb/merged.* pcb/*.ger pcb/*.gpi pcb/watch.drills.*

test1.o: test1.cc

#i2cmaster.o: i2cmaster.S i2cmaster.h
#	$(CC) $(CFLAGS) -x assembler-with-cpp -Wa,-gstabs -c $< -o $@

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
