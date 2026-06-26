MCU    = atmega328p
F_CPU  = 16000000UL
PORT   = /dev/ttyUSB0
BAUD   = 115200
PROG   = arduino

CC      = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu11

COMMON  = uart.c twi.c

.PHONY: all escaner captura inferencia flash-escaner flash-captura flash-inferencia clean

all: escaner captura inferencia

escaner: escaner.hex
escaner.hex: escaner_i2c.c $(COMMON) uart.h twi.h
	$(CC) $(CFLAGS) -o escaner.elf escaner_i2c.c $(COMMON)
	$(OBJCOPY) -O ihex -R .eeprom escaner.elf escaner.hex
	@avr-size escaner.elf

flash-escaner: escaner.hex
	avrdude -p $(MCU) -c $(PROG) -P $(PORT) -b $(BAUD) -U flash:w:escaner.hex:i

captura: captura.hex
captura.hex: captura.c $(COMMON) uart.h twi.h
	$(CC) $(CFLAGS) -o captura.elf captura.c $(COMMON)
	$(OBJCOPY) -O ihex -R .eeprom captura.elf captura.hex
	@avr-size captura.elf

flash-captura: captura.hex
	avrdude -p $(MCU) -c $(PROG) -P $(PORT) -b $(BAUD) -U flash:w:captura.hex:i

inferencia: inferencia.hex
inferencia.hex: inferencia.c $(COMMON) uart.h twi.h model.h
	$(CC) $(CFLAGS) -o inferencia.elf inferencia.c $(COMMON) -lm
	$(OBJCOPY) -O ihex -R .eeprom inferencia.elf inferencia.hex
	@avr-size inferencia.elf

flash-inferencia: inferencia.hex
	avrdude -p $(MCU) -c $(PROG) -P $(PORT) -b $(BAUD) -U flash:w:inferencia.hex:i

clean:
	rm -f *.elf *.hex
