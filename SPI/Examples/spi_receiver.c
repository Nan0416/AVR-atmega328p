#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mySPI.h"

mySPI spi;
void led(void);
const uchar data[] = {0x12, 0x36, 0x11};
ISR(SPI_STC_vect){
	uchar buf[3];
	spi_passive_receive(buf ,3);
	if(buf[0] == 0x12 && buf[1]==0x36 &&buf[2] == 0x11){
		led();
	}
}


int main(){
	init_spi();
	setup_spi();
	config_spi_operating_mode(SLAVE);
	led();
	sei();
	config_spi_enable(TRUE);
	config_spi_interrupt(TRUE);
	while(1);
	return 0;
}

void led(void){
	DDRD |= 0x20;
	PORTD |= 0x20;
	_delay_ms(1000);
	PORTD &= 0xDF;
	_delay_ms(1000);
}
