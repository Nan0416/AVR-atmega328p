#include "myExternal.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void setup_external_interrupt(EXT_INT no, EXT_MODE trigger_mode){
	cli(); // disable all interrupt
	unsigned char trigger;
	switch(trigger_mode){
		case RISING_EDGE: trigger = SENCE_CON_RISING_EDGE; break;
		case FALLING_EDGE: trigger = SENCE_CON_FALLING_EDGE; break;
		case EDGES: trigger = SENCE_CON_RF_EDGE; break;
		default: trigger = SENCE_CON_LOW_LEVEL; break;
	}
	if(no == EXT_INT_0){
		DDRD = DDRD & PORT_DIRECTION_INT0; 
		EICRA = EICRA | (trigger & SENCE_CON_INT0); 
		EIMSK = EIMSK | ENABLE_INT0; // open mask
	}else if(no == EXT_INT_1){
		DDRD = DDRD & PORT_DIRECTION_INT1;
		EICRA = EICRA | (trigger & SENCE_CON_INT1); 
		EIMSK = EIMSK | ENABLE_INT1; // open mask
	}else{
		DDRD = DDRD & PORT_DIRECTION_INT;
		EICRA = EICRA | (trigger & SENCE_CON_INT); 
		EIMSK = EIMSK | ENABLE_INT; // open mask
	}
	
}
