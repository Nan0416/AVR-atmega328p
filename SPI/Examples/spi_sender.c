#ifndef F_CPU
#define F_CPU 16000000UL
#endif //
#include <avr/io.h>
#include <util/delay.h>

#include "mySPI.h"

mySPI spi;
int main(){
	init_spi();
	setup_spi();
	config_spi_enable(TRUE);
	
	const uchar data[] = {0x12, 0x36, 0x11};
	while(1){
	
		_delay_ms(1000);
		spi_write_buffer(0, data, 3);
		
		
	}
	return 0;
}
