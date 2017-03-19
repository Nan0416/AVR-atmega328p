all:control_led.flash.hex
	avrdude -p m328p -c stk500v1 -b 57600 -P /dev/ttyUSB0 -U flash:w:control_led.flash.hex
control_led.flash.hex: control_led.elf
	avr-objcopy -j .text -j .data -O ihex control_led.elf control_led.flash.hex
control_led.elf: control_led.o libEXT.a
	avr-gcc -static -mmcu=atmega328p -o control_led.elf control_led.o -L. -lEXT
control_led.o: control_led.c
	avr-gcc -Wall -O1 -c -o control_led.o -mmcu=atmega328p control_led.c
