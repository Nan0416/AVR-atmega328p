#ifndef _COMMON_H__
#define _COMMON_H__

#include <avr/io.h>
#include <stdint.h>
#include "F_CPU.h"
#define INPUT 0x10
#define OUTPUT 0x11
#define PULLUP_INPUT 0x12

#define HIGH 1
#define LOW 0
#define ENABLE 0
#define DISABLE 1

#define boolean uint8_t

#define NULL 0
#define abs(x) (x)> 0?(x):(-x)

#define delay(ms) \
do{\
	while((ms)--){\
		_delay_ms(1); \
	}\
}while(0);

void pinBMode(uint8_t pin, uint8_t mode);
#define pinBGet(pin) (PINB & _BV(pin)) >> pin
#define pinBSet(pin, value) PORTB = (value > 0)? (PORTB | _BV(pin)):(PORTB & ~_BV(pin))

void pinCMode(uint8_t pin, uint8_t mode);
#define pinCGet(pin) (PINC & _BV(pin)) >> pin
#define pinCSet(pin, value) PORTC = (value > 0)? (PORTC | _BV(pin)):(PORTC & ~_BV(pin))

void pinDMode(uint8_t pin, uint8_t mode);
#define pinDGet(pin) (PIND & _BV(pin)) >> pin
#define pinDSet(pin, value) PORTD = (value > 0)? (PORTD | _BV(pin)):(PORTD & ~_BV(pin))


#define PinB0 0x00
#define PinB1 0x01
#define PinB2 0x02
#define PinB3 0x03
#define PinB4 0x04
#define PinB5 0x05
#define PinB6 0x06
#define PinB7 0x07

#define PinC0 0x08
#define PinC1 0x09
#define PinC2 0x0a
#define PinC3 0x0b
#define PinC4 0x0c
#define PinC5 0x0d
#define PinC6 0x0e
#define PinC7 0x0f

#define PinD0 0x10
#define PinD1 0x11
#define PinD2 0x12
#define PinD3 0x13
#define PinD4 0x14
#define PinD5 0x15
#define PinD6 0x16
#define PinD7 0x17

#define UNDEFINED 0x18
void pinMode(uint8_t pin, uint8_t mode);
uint8_t pinGet(uint8_t pin);
void pinSet(uint8_t pin, uint8_t value);


#endif //_COMMON_H__
