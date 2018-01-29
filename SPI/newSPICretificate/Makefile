# generate libSPI.a

all: mySPI.o
	avr-ar rcs libSPI.a mySPI.o
mySPI.o: mySPI.c _common.h mySPI.h
	avr-gcc -O1 -Wall -c -mmcu=atmega328p  mySPI.c
