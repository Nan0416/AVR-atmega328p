#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mySPI.h"

SPISettings spi;

char k[20];
int i = 0;

void led(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(100);
	PORTC &= 0xFE;
	_delay_ms(200);
}

ISR (SPI_STC_vect)
{
  char c = SPDR;
  if (c == '\n')
  {
    led();
    i = 0;
  }
  else if (c != '\n')
  {
    k[i] = c;
    i++;
  }
}


int main(){
	init_spi();
	spi.op_mode = SLAVE;
	setup_spi();
	
	sei();
	SPI_ENABLE_INTERRUPT;
	SPI_ENABLE;
	led();
	while(1);
	return 0;
}


