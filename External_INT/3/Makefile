all:interrupt1.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:interrupt1.flash.hex
interrupt1.flash.hex: interrupt1.elf
	avr-objcopy -j .text -j .data -O ihex interrupt1.elf interrupt1.flash.hex
interrupt1.elf: interrupt1.o
	avr-gcc -static -mmcu=atmega328p -o interrupt1.elf interrupt1.o
interrupt1.o: interrupt1.c
	avr-gcc -Wall -O1 -c -o interrupt1.o -mmcu=atmega328p interrupt1.c
