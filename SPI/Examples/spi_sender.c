#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>

#include "mySPI.h"

mySPI spi;
int main(){
	init_spi(&spi);
	setup_spi(&spi);
	config_spi_enable(&spi, TRUE);
	
	uchar data = 0x12;
	while(1){
		//PORTB &= ~0x02;
		spi_transfer_byte(&spi, 0, data);
		_delay_ms(1000);
		//PORTB |= 0x02;
		
		
	}
	return 0;
}
