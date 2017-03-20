
#include "alarm.h"

static uint64_t _remain_time__;

ISR(TIMER0_OVF_vect){
	alarm_us(_remain_time__);
}

void alarm_us(uint64_t time){
	_remain_time__ = time;
	if(_remain_time__ > 16384){
		_remain_time__ -= 16384;
		TCNT0 = 0x00;
		// enable interrupt overflow
		TIMSK0 = 0x01;
		// normal mode
		TCCR0A = 0x00;
		// P_1024 prescale
		TCCR0B = 0x05;
	}else if(_remain_time__ > 4096){
		_remain_time__ -= 4096;
		TCNT0 = 0x00;
		// enable interrupt overflow
		TIMSK0 = 0x01;
		// normal mode
		TCCR0A = 0x00;
		// P_1024 prescale
		TCCR0B = 0x04;
	}else if(_remain_time__ > 1024){
		_remain_time__ -= 1024;
		TCNT0 = 0x00;
		// enable interrupt overflow
		TIMSK0 = 0x01;
		// normal mode
		TCCR0A = 0x00;
		// P_1024 prescale
		TCCR0B = 0x03;
	}else if(_remain_time__ > 128){
		_remain_time__ -= 128;
		TCNT0 = 0x00;
		// enable interrupt overflow
		TIMSK0 = 0x01;
		// normal mode
		TCCR0A = 0x00;
		// P_1024 prescale
		TCCR0B = 0x02;
	}else{
		
		OCR0A = _remain_time__ << 1;
		TCNT0 = 0x00;
		// enable interrupt A, disable overflow
		TIMSK0 = 0x02;
		// ctc mode
		TCCR0A = 0x02;
		// P_8 prescale
		TCCR0B = 0x02;
	}
	
}
void clear_flag(void){
	if(TIFR0 & 0x02){
		TIFR0 |= 0x02;
	}
}

void alarm_ms(uint32_t time){
		
	alarm_us(time * 1000);	
}
void alarm_s(uint32_t time){
	alarm_us(time * 1000000);	
}
void alarm_min(uint32_t time){
	alarm_us(time * 60000000);	
}
