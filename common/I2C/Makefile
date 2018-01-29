all:main.flash.hex
	avrdude -p m328p -c stk500v1 -b 19200 -P /dev/ttyUSB1 -U flash:w:main.flash.hex
main.flash.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: common.o main.o AVRI2C.o
	avr-g++ -std=c++11 -mmcu=atmega328p -o main.elf common.o main.o AVRI2C.o
AVRI2C.o : AVRI2C.cpp AVRI2C.hpp
	avr-g++ -std=c++11 -Wall -O1 -c -o AVRI2C.o -mmcu=atmega328p AVRI2C.cpp
main.o: main.c
	avr-g++ -std=c++11 -Wall -O1 -c -o main.o -mmcu=atmega328p main.c
common.o: common.c common.h 
	avr-g++ -std=c++11 -Wall -O1 -c -o common.o -mmcu=atmega328p common.c
clean: 
	rm *.o *.elf *.hex *.gch
