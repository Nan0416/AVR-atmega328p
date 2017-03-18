#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mySPI.h"

mySPI spi;
void led(void);
ISR(SPI_STC_vect){
	
	if(spi_read_byte() == 0x12){
		led();
	}
}


int main(){
	init_spi(&spi);
	setup_spi(&spi);
	config_spi_operating_mode(&spi, SLAVE);
	
	led();
	sei();
	config_spi_enable(&spi, TRUE);
	config_spi_interrupt(&spi, TRUE);
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
