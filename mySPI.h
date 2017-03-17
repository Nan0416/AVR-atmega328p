#ifndef F_CPU
#define F_CPU 16000000UL
#endif

typedef unsigned char uint8_t;
#include <avr/io.h>
#define CSN_ENABLE PORTD &= 0xBF
#define CSN_DISABLE PORTD |= 0x40
void setup_spi(void);
uint8_t spi_transfer(uint8_t data);
