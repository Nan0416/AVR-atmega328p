#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "myNRF24L01v2.h"
#include "myExternal.h"
#include "mySPI.h"


uint8_t data[20];
uint8_t answer[] = {0x10, 0x89, 0x97, 0x23};


void let_indicator(void);
void let_indicator_fast(void);
uint8_t compare(uint8_t d1[], uint8_t d2[] , uint8_t len);



NRF24L01P config;



ISR(INT0_vect){
	let_indicator();
	memset(data, 0x00, 20);
	recv(&config, data, 20);
	if(compare(answer, data, 4) == 1){
		let_indicator_fast();
	}
}


int main(){	
	
	init_rf24l01p(&config);
	setup_spi();
	setup_rf24l01p(&config);
	/* customize your nrf24l01+ with config_ functions */
	
	
	
	
	uint8_t addr [] =  {0x30, 0x30, 0x30, 0x30, 0x31};
	
	
	config_listening_addr(&config, addr);
	setup_external_interrupt(EXT_INT_0, FALLING_EDGE);
	sei();
	while(1);
	return 0;
}


void let_indicator(void){
	DDRD |= 0x20;
	PORTD |= 0x20;
	_delay_ms(1000);
	PORTD &= 0xDF;
	_delay_ms(1000);
}
void let_indicator_fast(void){
	DDRD |= 0x20;
	PORTD |= 0x20;
	_delay_ms(100);
	PORTD &= 0xDF;
	_delay_ms(100);
}
uint8_t compare(uint8_t d1[], uint8_t d2[] , uint8_t len){
	while(len--){
		if(*d1++ != *d2++){
			return 0;
		}
	}
	return 1;
}
