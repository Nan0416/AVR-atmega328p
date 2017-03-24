
# avrdude -p m328p -c stk500v1 -b 57600 -P /dev/ttyUSB0 -U flash:w:main.flash.hex // download
all: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.flash.hex
main.elf: main.o
	avr-gcc -mmcu=atmega328p -o main.elf main.o lcd.o number2char.o
main.o: main.c
	avr-gcc -O1 -Wall -c -mmcu=atmega328p -o main.o main.c 
lcd.o: lcd.c
	avr-gcc -O1 -Wall -c -mmcu=atmega328p -o lcd.o lcd.c
number2char.o: number2char.c
	avr-gcc -O1 -Wall -c -mmcu=atmega328p -o number2char.o number2char.c
clean:
	rm main.elf main.o main.flash.hex
