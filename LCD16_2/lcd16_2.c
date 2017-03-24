#ifndef F_CPU 
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include "_common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define LCD_CMD 0
#define LCD_DATA 1
#define LCD_LINE_MAX 40
#define LCD_SHOW_LINE_MAX 16
#define LCD_LINES 2
#define CLEAR lcd_write(LCD_CMD, 0x02);_delay_ms(10) // reset data pointer
#define CLEAR_SCREEN lcd_write(LCD_CMD, 0x01);_delay_ms(10)

/**
 * LCD cannot read in control register's data
 * 
 * 
 * 
 * **/

typedef void (*fp)(void); 
typedef union lcd_pin_assignment{
	PIN_ADDR data_pins[8];
	MEGA_PORT port;
}lcd_pin_assignment;

typedef enum CURSOR{
	OFF = 0,
	FLUSH = 1,
	ON = 2
}CURSOR;

typedef struct myLCD{
	PIN_ADDR ce;
	PIN_ADDR rw;
	PIN_ADDR cmd_data;
	pin_type data_port_type;
	lcd_pin_assignment data_addr;
	CURSOR cursor;
	uint8_t show_on;
}myLCD;

typedef struct lcd_show_data{
	const char * data;
	uint32_t beginning;
	uint32_t data_len;
}lcd_show_data;

myLCD lcd;
void led(){
	DDRB |= 0x20;
	PORTB |= 0x20;
	_delay_ms(1000);
	PORTB &= ~0x20;
	_delay_ms(1000);
}


/***********/
void init_lcd();
void setup_lcd();
void config_show(uint8_t on, CURSOR status);
void config_pin(PIN_ADDR lcd_pin, uint8_t value);
void lcd_write(uint8_t cmd_data_s, uint8_t value);
void write_out(uint8_t data);
void _bit_writer_helper(uint8_t  data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin);
uint8_t read_in(void);
void _bit_reader_helper(uint8_t * data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin);
/***********/
void init_lcd(){
	lcd.ce.port = PD;
	lcd.ce.pin = P6;
	lcd.rw.port = PD;
	lcd.rw.pin = P5;
	lcd.cmd_data.pin = P4;
	lcd.cmd_data.port = PD;

	lcd.data_addr.data_pins[0].pin = P0;
	lcd.data_addr.data_pins[0].port = PC;
	lcd.data_addr.data_pins[1].pin = P1;
	lcd.data_addr.data_pins[1].port = PC;
	lcd.data_addr.data_pins[2].pin = P2;
	lcd.data_addr.data_pins[2].port = PC;
	lcd.data_addr.data_pins[3].pin = P3;
	lcd.data_addr.data_pins[3].port = PC;
	
	lcd.data_addr.data_pins[4].pin = P0;
	lcd.data_addr.data_pins[4].port = PB;
	lcd.data_addr.data_pins[5].pin = P1;
	lcd.data_addr.data_pins[5].port = PB;
	lcd.data_addr.data_pins[6].pin = P2;
	lcd.data_addr.data_pins[6].port = PB;
	lcd.data_addr.data_pins[7].pin = P3;
	lcd.data_addr.data_pins[7].port = PB;
	lcd.data_port_type = pin;
	lcd.show_on = 0;
	lcd.cursor = FLUSH;

}

void setup_lcd(){
	_delay_ms(15);
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(64); // 5ms
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(50); // 5ms
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(20); // 5ms
	lcd_write(LCD_CMD, 0x01);  // clear the screen
	config_show(0, OFF);
	CLEAR_SCREEN;
	CLEAR;
}

void config_show(uint8_t on, CURSOR status){
	uint8_t value = 0x08;
	lcd.show_on = !(!on);
	value |= lcd.show_on << 2;
	value |= status;
	lcd_write(LCD_CMD, value);
	lcd.cursor = status;
}

void config_pin(PIN_ADDR lcd_pin, uint8_t value){
	if(value == HIGH){
		switch(lcd_pin.port){
			case PB: DDRB |= _BV(lcd_pin.pin); PORTB |= _BV(lcd_pin.pin);break;
			case PC: DDRC |= _BV(lcd_pin.pin); PORTC |= _BV(lcd_pin.pin);break;
			case PD: DDRD |= _BV(lcd_pin.pin); PORTD |= _BV(lcd_pin.pin);break; 
			default: break;
		}
	}else{
		switch(lcd_pin.port){
			case PB: DDRB |= _BV(lcd_pin.pin); PORTB &= ~_BV(lcd_pin.pin);break;
			case PC: DDRC |= _BV(lcd_pin.pin); PORTC &= ~_BV(lcd_pin.pin);break;
			case PD: DDRD |= _BV(lcd_pin.pin); PORTD &= ~_BV(lcd_pin.pin);break; 
			default: break;
		}
	}
}
void lcd_write(uint8_t cmd_data_s, uint8_t value){
	/* slow close interrupt*/
	uint8_t interrupt_open = SREG & 0x80;
	cli();
	if(cmd_data_s){
		// high data
		config_pin(lcd.cmd_data, HIGH);
	}else{
		config_pin(lcd.cmd_data, LOW);
	}
	config_pin(lcd.rw, LOW);
	write_out(value);
	_delay_us(50); // 40ns, here is us
	config_pin(lcd.ce, HIGH);
	_delay_us(150); //
	//while(1);
	config_pin(lcd.ce, LOW);
	_delay_us(50); // 10ns
	if(interrupt_open){
		sei();
	}
	_delay_us(400);
	// to next enable 400 ns delay
}

void write_out(uint8_t data){
	if(lcd.data_port_type == port){
		switch(lcd.data_addr.port){
			case PB: DDRB = 0xFF; PORTB = data;break;
			case PC: DDRC = 0xFF; PORTC = data;break;
			case PD: DDRD = 0xFF; PORTC = data;break;
			default: break;
		}
	}else{
		int i = 0;
		for(; i < 8; i++){
			_bit_writer_helper(data, i, lcd.data_addr.data_pins[i].port, lcd.data_addr.data_pins[i].pin);
		}
	}
}

void _bit_writer_helper(uint8_t  data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin){
	uint8_t data_temp = (data >> position) & 0x01;
	switch(_port){
			case PB: DDRB |= _BV(_pin); PORTB = (PORTB & ~_BV(_pin)) | (data_temp << _pin); break;
			case PC: DDRC |= _BV(_pin); PORTC = (PORTC & ~_BV(_pin)) | (data_temp << _pin); break;
			case PD: DDRD |= _BV(_pin); PORTD = (PORTD & ~_BV(_pin)) | (data_temp << _pin); break;
			default: break ;
		}
	

}

uint8_t read_in(void){
	uint8_t data = 0x00;
	if(lcd.data_port_type == port){
		switch(lcd.data_addr.port){
			case PB: DDRB = 0x00; return PORTB;
			case PC: DDRC = 0x00; return PORTC; 
			case PD: DDRD = 0x00; return PORTD;
			default: return 0x00;
		}
	}else{
		int i = 0;
		for(; i < 8; i++){
			_bit_reader_helper(&data, i, lcd.data_addr.data_pins[i].port, lcd.data_addr.data_pins[i].pin);
		}
	}
	return data;
}
void _bit_reader_helper(uint8_t * data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin){
	uint8_t data_temp = 0x00;
	switch(_port){
			case PB: DDRB &= ~_BV(_pin); data_temp = (PORTB & _BV(_pin)) >> _pin; break;
			case PC: DDRC &= ~_BV(_pin); data_temp = (PORTC & _BV(_pin)) >> _pin; break;
			case PD: DDRD &= ~_BV(_pin); data_temp = (PORTD & _BV(_pin)) >> _pin; break;
			default: return ;
		}
	*data = (*data & ~_BV(position)) | (data_temp << position);

}
/* useful for two independent data */
void lcd_rolling_perpendicular(const char * data, uint8_t len, uint8_t initial_line, uint8_t repeat){
	lcd_write(LCD_CMD, 0x06); // pointer auto add 1
	uint8_t show_end = 0;
	uint8_t i = 0;
	if(initial_line == 1){
		lcd_write(LCD_CMD, 0x80);
		show_end = (len - 2* LCD_LINE_MAX) > 0 ? 2 * LCD_LINE_MAX: len;
		for(; i < show_end; i++){
			if(i > 0x27){
				break;
			}
			lcd_write(LCD_DATA, data[i]);
			
		}
		if(i == 0x28){
			lcd_write(LCD_CMD, 0xC0);
			for(; i< show_end; i++){
				lcd_write(LCD_DATA, data[i]);
			}
		}
	
	}else{
		lcd_write(LCD_CMD, 0xC0);
		show_end = (len -  LCD_LINE_MAX) > 0 ? LCD_LINE_MAX: len;
		for(; i < show_end; i++){
			lcd_write(LCD_DATA, data[i]);
		}
	}
	
	
}


void update_vertical(lcd_show_data * data){
	CLEAR;
	CLEAR_SCREEN;
	lcd_write(LCD_CMD, 0x06); // pointer auto add 1
	lcd_write(LCD_CMD, 0x80);
	
	uint8_t line_1_end = _min(data->beginning + LCD_SHOW_LINE_MAX, data->data_len);
	uint8_t i = data->beginning;
	for(; i < line_1_end;i++){
		lcd_write(LCD_DATA, data->data[i]);
	}
	data->beginning = line_1_end;
	i = data->beginning;
	if(data->beginning == data->data_len){
		return;
	}
	lcd_write(LCD_CMD, 0xC0);
	uint8_t line_2_end = _min(data->beginning + LCD_SHOW_LINE_MAX, data->data_len);
	for(; i < line_2_end;i++){
		lcd_write(LCD_DATA, data->data[i]);
	}
	data->beginning = line_2_end;
}

void lcd_change_one_byte(char u, uint8_t row, uint8_t col){
	if(row == 1 && col <= 40){
		lcd_write(LCD_CMD, 0x80 + col - 1);
		
	}else if(row == 2 && col <= 40){
		lcd_write(LCD_CMD, 0xC0 + col - 1);
	}else{
		return;
	}
	lcd_write(LCD_DATA, u);

}
void lcd_show_num(int num){
	CLEAR;
	CLEAR_SCREEN;
	char *s;
	uint8_t data_len;
	if(num > 99){
		data_len = 3;
		s = (char *) malloc(3);
	}else if(num > 9){
		data_len = 2;
		s = (char *) malloc(2);
	}else{
		data_len = 1;
		s = (char *) malloc(1);
	}
	itoa(num,s,10);
	lcd_rolling_perpendicular(s, data_len, 1, 0);
	
}
int main(){
	init_lcd();
	led();
	setup_lcd();
	const char * data = "In computer user interfaces, a cursor is an indicator used to show the current position for user interaction on a computer monitor or other display device that will respond to input from a text input or pointing device. The mouse";// cursor is also called a pointer, owing to its resemblance in usage to a pointing stick.";
	config_show(1, FLUSH);
	
	lcd_show_data msg;
	msg.data = data;
	msg.beginning = 0;
	msg.data_len = strlen(data);
	//lcd_rolling_perpendicular(data, 70, 1, 0);
	while(msg.data_len - msg.beginning > 0 ){
		lcd_show_num(msg.data_len - msg.beginning);
		_delay_ms(1000);
		update_vertical(&msg);
		_delay_ms(2000);
	}
	while(1);
	
}
