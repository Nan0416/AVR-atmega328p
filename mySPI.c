#include "mySPI.h"

void setup_spi(void){
	/* choose PD7 as CE, PD6 as CSN (~SS) */
	DDRD |= 0xC0;
	/* configure MOSI, SCK and MISO*/
	DDRB |= 0x2C;
	DDRB &= 0xEF;
	/* disable csn set mega SS as output and high*/
	
	CSN_DISABLE;
	PORTB |= 0x04;
	/* master mode, and enable spi*/
	SPCR |= 0x50;
}

uint8_t spi_transfer(uint8_t data){
	SPDR = data;
	while (!(SPSR & 0x80));
	return SPDR;
}
