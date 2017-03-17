#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "myNRF24L01v2.h"
#include "mySPI.h"

NRF24L01P config;

int main(){	
	init_rf24l01p(&config);
	setup_spi();
	setup_rf24l01p(&config);
	uint8_t addr [] = {0x30, 0x30, 0x30, 0x30, 0x31};
	uint8_t data[] = {0x10, 0x89, 0x97, 0x23};
	config_destination_addr(&config, addr);
	
	while(1){
		send(&config, data, 4);
		_delay_ms(1000);
	}
	return 0;
}
