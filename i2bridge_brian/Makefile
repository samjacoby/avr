CC = avr-gcc
OBJCOPY = avr-objcopy

OBJECTS = main.o uart.o twimaster.o
GENERATED_SOURCES = 
APPLICATION = i2cbridge
MCU = atmega168
F_CPU = 20000000UL

CFLAGS=-mmcu=$(MCU) -Wall -DF_CPU=$(F_CPU) -I. -funsigned-char -funsigned-bitfields \
	   -fpack-struct -fshort-enums -Os
LDFLAGS=-mmcu=$(MCU)

all: $(GENERATED_SOURCES) $(APPLICATION).hex

clean:
	rm -f $(GENERATED_SOURCES) *.hex *.elf *.o *.bin tools/*.pyc

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(APPLICATION).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	avr-size -C --mcu=$(MCU) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	avrdude -p m168 -c avrispmkII -P usb -e -U flash:w:$(APPLICATION).hex

.PHONY: all clean

