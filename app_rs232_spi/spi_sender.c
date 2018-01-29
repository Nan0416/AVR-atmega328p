#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>

#include "mySPI.h"

SPISettings spi;


void led(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(5);
	PORTC &= 0xFE;
	_delay_ms(10);
}

int main(){
	uint8_t slave_id = 0;
	init_spi();
	setup_spi();
	slave_id = add_slave(PB, P2);
	SPI_ENABLE;
	char * p = "Hello\n";
	
	while(1){
		led();
		
		spi_send_buffer(slave_id, p);
		_delay_ms(8);
	}
	return 0;
}
