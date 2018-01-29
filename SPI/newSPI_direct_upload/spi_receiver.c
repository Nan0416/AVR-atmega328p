#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mySPI.h"

mySPI spi;
//volatile uint8_t index;
//volatile uint8_t buf[3];
void led3(void);
ISR(SPI_STC_vect){
	uint8_t data = SPDR;
	//index++;
	if(data == 0x11){
		led3();
	}
	SPDR = 0x12;
	
}


int main(){
	init_spi();
	//spi.spi_on = TRUE;
	setup_spi();
	config_spi_operating_mode(SLAVE);
	led3();
	_delay_ms(1000);
	//index = 0;
	sei();
	config_spi_interrupt(TRUE);
	config_spi_enable(TRUE);
	led3();
	while(1);
	return 0;
}

void led3(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(100);
	PORTC &= 0xFE;
	_delay_ms(500);
}
