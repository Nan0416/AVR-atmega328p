#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>

#include "mySPI.h"

mySPI spi;


void led2(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(100);
	PORTC &= 0xFE;
	_delay_ms(500);
}

int main(){
	uint8_t slave_id = 0;
	init_spi();
	setup_spi();
	slave_id = add_slave(PB, P0);
	config_spi_enable(TRUE);
	const uint8_t data[] = {0x12, 0x36, 0x11};
	
	while(1){
		led2();
		_delay_ms(1000);
		//spi_send_buffer(slave_id, data, 3);
		spi_send_cmd(slave_id, 0x11);
		
	}
	return 0;
}
