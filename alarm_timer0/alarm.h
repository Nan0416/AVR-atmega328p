


/* 16MHz clock
 * Prescale  timer_frequency  resolution  max
 * 1		 16               0.0625 us   16 us
 * 8         2                0.5 us      128 us
 * 64        250 KHz          4 us        1024 us
 * 256       62.5 KHz         16 us       4096 us = 4.096 ms
 * 1024      15625 Hz         64 us       16384 us = 16.384 ms
 * 
 * 
 * 
 * 
 * */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#if F_CPU != 16000000UL
#error F_CPU not suitable
#endif

// based on AVR atmega 328p timer0
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/io.h>


// uint64MAX 18446744073709551616U

/* alarm_us is not stacked */
/* only once effect */
/* in the beginning of compare match A interrupt routine, MUST call either alarm_pause or alarm_us */
void alarm_us(uint64_t time);
void alarm_ms(uint32_t time); // 4294967296
void alarm_s(uint32_t time);
void alarm_min(uint32_t time);

void clear_flag(void);
#define alarm_pause() TCCR0B = 0x00


