#include "myLCD.h"
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
	_delay_ms(5);
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(5); // 5ms
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(5); // 5ms
	lcd_write(LCD_CMD, 0x38);
	_delay_ms(5); // 5ms
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
void lcd_update_line(lcd_show_data * data, uint8_t line, uint8_t direction, uint8_t step){
	/* windows goes that direction */
	//CLEAR;
	//CLEAR_SCREEN;
	lcd_write(LCD_CMD, 0x06);
	if(line == LCD_LINE_1){
		lcd_write(LCD_CMD, 0x80);
	}else if(line == LCD_LINE_2){
		lcd_write(LCD_CMD, 0xc0);
	}
	uint8_t end;
	uint8_t begin = data->beginning;
	uint8_t i = 0;
	if(direction == LCD_RIGHT){
		begin += step;
		end = _min(data->beginning + LCD_SHOW_LINE_MAX, data->data_len);
		if(begin < data->data_len){
			data->beginning = begin;
			for(i = begin; i < end; i++){
				lcd_write(LCD_DATA, data->data[i]);
			}
			
			
		}
	}else if(direction == LCD_LEFT){
		begin -= step;
		end = _min(data->beginning + LCD_SHOW_LINE_MAX, data->data_len);
		if(begin >= 0){
			data->beginning = begin;
			for(i = begin; i < end; i++){
				lcd_write(LCD_DATA, data->data[i]);
			}
		}
	}
}

void lcd_update_vertical(lcd_show_data * data){
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
void lcd_show_num(int num, int delay_ms){
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
	lcd_show_data msg;
	msg.data_len = data_len;
	msg.data = s;
	msg.beginning = 0;
	lcd_update_line(& msg, LCD_LINE_1, LCD_RIGHT, 0);
	while(delay_ms --){
		_delay_ms(1);
	}
	free(s);
	
}
