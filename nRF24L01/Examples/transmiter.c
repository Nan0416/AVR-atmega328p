#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "myNRF24L01v2.h"
#include "mySPI.h"

NRF24L01P rf24;
mySPI spi;
int main(){	
	init_spi();
	init_rf24l01p();
	
	setup_spi();
	config_spi_enable(TRUE);
	
	setup_rf24l01p();
	uint8_t addr [] = {0x30, 0x30, 0x30, 0x30, 0x31};
	uint8_t data[] = {0x10, 0x89, 0x97, 0x23};
	config_destination_addr( addr);
	while(1){
		send(data, 4);
		flush_rx();
		flush_tx();
		_delay_ms(1000);
	}
	return 0;
}
