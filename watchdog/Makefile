all:watchdogInterrupt.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:watchdogInterrupt.flash.hex
watchdogInterrupt.flash.hex: watchdogInterrupt.elf
	avr-objcopy -j .text -j .data -O ihex watchdogInterrupt.elf watchdogInterrupt.flash.hex
watchdogInterrupt.elf: watchdogInterrupt.o
	avr-gcc -static -mmcu=atmega328p -o watchdogInterrupt.elf watchdogInterrupt.o
watchdogInterrupt.o: watchdogInterrupt.c
	avr-gcc -Wall -O1 -c -o watchdogInterrupt.o -mmcu=atmega328p watchdogInterrupt.c
