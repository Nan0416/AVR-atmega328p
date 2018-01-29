#define F_CPU 16000000UL
#include <avr/io.h>
#include "myRS232.h"
#include "mySPI.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// 
struct RS232Data data;
SPISettings spi;
void led(void){
	DDRC |= 0x01;
	PORTC |= 0x01;
	_delay_ms(100);
	PORTC &= 0xFE;
	_delay_ms(200);
}

// maximum 8ms delay
ISR (SPI_STC_vect)
{
	// we can only read data from routine
  char c = SPDR;
  if (c != '\n' && data.len < MAX_DATA_LEN)
  {
	data.data[data.len] = c;
	data.len++;
    
  }else if(c == '\n' || data.len >= MAX_DATA_LEN){
	//send to the computer
	send(&data);
	//led();
	data.len = 0;
	data.offset = 0;
  }
  
}

int main(){
	data.len = 0;
	data.offset = 0;
	init_rs232();
	rs232_DISABLE_INTERRUPT;
	enable_rs232();
	
	init_spi();
	spi.op_mode = SLAVE;
	setup_spi();
	SPI_ENABLE_INTERRUPT;
	SPI_ENABLE;
	
	sei();
	led();
	while(1);
	
	return 0;
}


