# generate libSPI.a

all: myRF24.o
	avr-ar rcs libRF24.a myRF24.o
myRF24.o: myNRF24L01v2.c _common.h myNRF24L01v2.h nRF24L01.h
	avr-gcc -O1 -Wall -c -o myRF24.o -mmcu=atmega328p  myNRF24L01v2.c
