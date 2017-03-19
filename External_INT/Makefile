# generate libEXT.a

all: myEXT.o
	avr-ar rcs libEXT.a myEXT.o
myEXT.o: myExternal.c _common.h myExternal.h
	avr-gcc -O1 -Wall -c -o myEXT.o -mmcu=atmega328p  myExternal.c
