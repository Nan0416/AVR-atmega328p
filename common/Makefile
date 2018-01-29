all:main.flash.hex
	avrdude -p m328p -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex
main.flash.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: test.o common.o ST7735.o SPI.o UI.o
	avr-g++ -std=c++11 -mmcu=atmega328p -o main.elf common.o test.o ST7735.o SPI.o UI.o
#.hpp include inline
test.o: test.cpp SPI.hpp F_CPU.h ST7735.hpp
	avr-g++ -std=c++11 -Wall -O1 -c -mmcu=atmega328p test.cpp SPI.hpp ST7735.hpp
UI.o: UI.cpp UI.hpp F_CPU.h
	avr-g++ -std=c++11 -Wall -O1 -c  -mmcu=atmega328p UI.cpp UI.hpp
ST7735.o: ST7735.cpp ST7735.hpp F_CPU.h
	avr-g++ -std=c++11 -Wall -O1 -c  -mmcu=atmega328p ST7735.cpp ST7735.hpp
SPI.o: SPI.cpp SPI.hpp F_CPU.h
	avr-g++ -std=c++11 -Wall -O1 -c -o SPI.o -mmcu=atmega328p SPI.cpp
common.o: common.c common.h 
	avr-g++ -std=c++11 -Wall -O1 -c -o common.o -mmcu=atmega328p common.c
