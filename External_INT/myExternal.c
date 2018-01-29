#include "myExternal.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/*
 * initialize an external interrupt data structure.
 * */
void init_ext(myEXT * ext, EXT_INT int_no){
	ext->int_no = int_no;
	ext->int_mode = RISING_EDGE;
	ext->enabled = FALSE;
	
}
void setup_ext(myEXT * ext){
	config_ext_mode(ext, ext->int_mode);
	config_ext_enable(ext, ext->enabled);
}

/*
 * set trigger mode, failing edge, rising edge, level low
 * */
void config_ext_mode(myEXT * ext, EXT_MODE mode){
	EICRA |= ext->int_no & mode; 
	ext->int_mode = mode;	
}

/*
 * enable/disable
 * setup pin direction
 * */
void config_ext_enable(myEXT * ext, BOOL enable){
	if(enable == TRUE){
		if(ext->int_no == EXT_INT_0){
			DDRD &= PORT_DIRECTION_INT0; 
			EIMSK |= ENABLE_INT0; // open mask
		}else{
			DDRD &= PORT_DIRECTION_INT1; 
			EIMSK |= ENABLE_INT1; // open mask
		}
	}else{
		if(ext->int_no == EXT_INT_0){
			
			EIMSK &= ~ENABLE_INT0; 
		}else{
			
			EIMSK &= ~ENABLE_INT1; 
		}
	}
	ext->enabled = enable;
}

/*
 * clear the buffered bit
 * */
void clear_ext_irq_flag(myEXT * ext){
	if(ext->int_no == EXT_INT_0){
		EIFR = 0x01;
	}else{
		EIFR = 0x02;
	}
}

/*
 * check if bit is set
 * */
BOOL check_irq_flag(myEXT * ext){
	if(ext->int_no == EXT_INT_0){
		if(EIFR & 0x01){
			return TRUE;
		}else{
			return FALSE;
		}
	}else{
		if(EIFR & 0x02){
			return TRUE;
		}else{
			return FALSE;
		}
	}
}

