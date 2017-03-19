#include "myExternal.h"
#include <avr/io.h>
#include <avr/interrupt.h>



void init_ext(myEXT * ext){
	ext->int_no = EXT_INT_0;
	ext->int_mode = RISING_EDGE;
	ext->enabled = FALSE;
}
void setup_ext(myEXT * ext){
	config_ext_mode(ext, ext->int_mode);
	config_ext_enable(ext, ext->enabled);
}
void config_ext_mode(myEXT * ext, EXT_MODE mode){
	
	EICRA |= ext->int_no & mode; 
	ext->int_mode = mode;	
	
}
void config_ext_enable(myEXT * ext, BOOL enable){
	if(enable == TRUE){
		if(ext->int_no == EXT_INT_0){
			DDRD &= DDRD & PORT_DIRECTION_INT0; 
			EIMSK |= ENABLE_INT0; // open mask
		}else if(ext->int_no == EXT_INT_1){
			DDRD &= PORT_DIRECTION_INT1; 
			EIMSK |= ENABLE_INT1; // open mask
		}
	}else{
		if(ext->int_no == EXT_INT_0){
			
			EIMSK &= ~ENABLE_INT0; 
		}else if(ext->int_no == EXT_INT_1){
			
			EIMSK &= ~ENABLE_INT1; 
		}
	}
	ext->enabled = enable;
}

void config_ext_no(myEXT * ext, EXT_INT int_no){
	ext->int_no = int_no;
	setup_ext(ext);

}
void clear_ext_irq_flag(myEXT * ext){
	if(ext->int_no == EXT_INT_0){
		EIFR = 0x01;
	}else if(ext->int_no == EXT_INT_1){
		EIFR = 0x02;
	}
}
