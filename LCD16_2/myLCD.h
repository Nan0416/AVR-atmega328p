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
#define LCD_LINE_1 12
#define LCD_LINE_2 13
#define LCD_LEFT 1
#define LCD_RIGHT 2
#define CLEAR lcd_write(LCD_CMD, 0x02);_delay_ms(4) // reset data pointer
#define CLEAR_SCREEN lcd_write(LCD_CMD, 0x01);_delay_ms(4)

/**
 * LCD cannot read in control register's data
 * 
 * 
 * 
 * **/


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

extern myLCD lcd;



void init_lcd();

void setup_lcd();

void config_show(uint8_t on, CURSOR status);
void config_pin(PIN_ADDR lcd_pin, uint8_t value);
void lcd_write(uint8_t cmd_data_s, uint8_t value);
void write_out(uint8_t data);
void _bit_writer_helper(uint8_t  data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin);
uint8_t read_in(void);
void _bit_reader_helper(uint8_t * data, uint8_t position, MEGA_PORT _port, MEGA_PIN _pin);
void lcd_update_line(lcd_show_data * data, uint8_t line, uint8_t direction, uint8_t step);
void lcd_update_vertical(lcd_show_data * data);
void lcd_change_one_byte(char u, uint8_t row, uint8_t col);
void lcd_show_num(int num, int delay_ms);



