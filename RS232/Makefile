all:main.flash.hex
	avrdude -p m328p -c stk500v1 -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex
main.flash.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: main.o rs232.o 
	avr-gcc -mmcu=atmega328p -o main.elf rs232.o main.o
rs232.o: myRS232.c myRS232.h
	avr-gcc -Wall -O1 -c -o rs232.o -mmcu=atmega328p myRS232.c
main.o: main.c
	avr-gcc -Wall -O1 -c -o main.o -mmcu=atmega328p main.c
