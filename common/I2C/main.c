#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include "AVRI2C.hpp"
#include <stdint.h>
#include <string.h>
TWISlave sl;
char gdata[32];
void s(){
	
	sl.twi_transmit((const uint8_t *)gdata, strlen(gdata));
}
void r(uint8_t* data, int x){
	int i = 0;
	for(; i < x && i < 31; i++){
		gdata[i] = data[i];
	}
	data[i] = '\n';
}
int slave(){
	
	sl.twi_init();
	sl.twi_setAddress(8);
	//uint8_t data[16];
	DDRB |= 0x20;
	//twi_setAddress(8);
	sl.twi_attachSlaveTxEvent( s );
	sl.twi_attachSlaveRxEvent( r );
	while(1);
	return 0;
}
//master receive and master send
int master(){
	TWIMaster ms;
	ms.twi_init();
	ms.twi_setFrequency(80);
	uint8_t data[16];
	delay(1000);
	//twi_setAddress(8);
	do{ 
		 DDRB |= 0x20;
		PORTB |= 0x20;
		ms.twi_readFrom((uint8_t)8, data, 12, 0);
		delay(1000);
		ms.twi_writeTo((uint8_t)8, (uint8_t*)data, 12, 1, 1);
		DDRB |= 0x20;
		PORTB &= ~0x20;
		delay(1000);
	}while(1);
	return 0;
}
int main(){
	return slave();
}
