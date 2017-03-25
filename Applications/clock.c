#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define END_YEAR 2100
#define START_YEAR 2001
#define START_DATE 1 // 2001 Jan 1 is Monday

#define TIME_OFFSET 6
#define WEEK_OFFSET 1
#define MONTH_DISPLAY_OFFSET 2
#define DAY_OFFSET 6
#define YEAR_OFFSET 9
#include "alarm.h"
#include "myLCD.h"
#include <stdint.h>
#include <avr/interrupt.h>
typedef struct _time{
	uint8_t second; // 0-59
	uint8_t minute; // 0-59
	uint8_t hour; // 0-23
	uint8_t date; // 1-7
	uint8_t day; // 1-31
	uint8_t month; // 1-12
	uint32_t year; // 2001 - 2100
}_time;

myLCD lcd;
_time time;
char time_[16];
char date[16];
lcd_show_data line1;
lcd_show_data line2;
void init_time(void);
void update_1_s(void);
void update_day(_time * time, uint8_t target);
void generate_str(_time * time, char time_[], char date[]);
char *month[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "OCT", "NOV", "DEC"};
char *week[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
void led(){
		DDRB |= 0x20;
		PORTB |= 0x20;
		_delay_ms(300);
		PORTB |= 0x20;
		_delay_ms(200);
		
}
ISR(TIMER0_COMPA_vect){
	alarm_s(1);
	//led();
	
	//lcd_show_num(time.second, 0);
	CLEAR;
	CLEAR_SCREEN;
	generate_str(&time, time_, date);
	lcd_update_line(&line1, LCD_LINE_1, LCD_RIGHT, 0);
	lcd_update_line(&line2, LCD_LINE_2, LCD_RIGHT, 0);
	update_1_s();
}
int main(){
	uint8_t i = 0;
	for(; i < 16; i++){
		time_[i] = ' ';
		date[i] = ' ';
	}
	line1.data_len = 16;
	line1.beginning = 0;
	line1.data = time_;
	line2.data_len = 16;
	line2.beginning = 0;
	line2.data = date;
	sei();
	init_lcd();
	setup_lcd();
	config_show(1, OFF);
	alarm_s(1);
	init_time();
	while(1){
		//led();
	}
}

void init_time(void){
	time.second = 0;
	time.minute = 35;
	time.hour = 21;
	time.day = 24;
	time.month = 3;
	time.year = 2017;
	time.date = 5;
}
void update_1_s(void){
	time.second += 1;
	if(time.second >= 60){
		time.second = 0;
		time.minute += 1;
	}else{
		return;
	}
	if(time.minute >= 60){
		time.minute = 0;
		time.hour += 1;
	}else{
		return;
	}
	if(time.hour >= 24){
		time.hour = 0;
		time.day += 1;
	}else{
		return;
	}
	uint8_t leap_year = (((time.year % 4 == 0) && (time.year % 100 !=0))||(time.year % 400 == 0));
	uint8_t feb_days = leap_year? 29:28;
	switch(time.month){
		case 1: 
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: update_day(&time, 31); break;
		case 4:
		case 6:
		case 9:
		case 11: update_day(&time, 30);break;
		case 2: update_day(&time, feb_days); break;
		default: break;
	}
}
void update_day(_time * time, uint8_t target){
	if(time->day > target){
		time->day = 1;
		time->month += 1;
	}else{
		return;
	}
	if(time->month > 12){
		time->month = 1;
		time->year += 1;
	}else{
		return;
	}
	if(time->year > END_YEAR){
		time->second = 0;
		time->minute = 0;
		time->hour = 0;
		time->day = 1;
		time->month = 1;
		time->year = START_YEAR;
		time->date = START_DATE;
	}else{
		return;
	}
	
}
void generate_str(_time * time, char time_[], char date[]){
	uint8_t i = 0;
	
	for(; i< 3; i++){
		time_[i + WEEK_OFFSET] = week[time->date - 1][i];
		date[i + MONTH_DISPLAY_OFFSET] = month[time->month - 1][i];
	}
	char temp[2];
	char temp_1;
	if(time->hour > 9){
		itoa(time->hour, temp ,10);
		time_[TIME_OFFSET] = temp[0];
		time_[TIME_OFFSET+1] = temp[1];
		time_[TIME_OFFSET+2] = ':';
	}else{
		time_[TIME_OFFSET] = '0';
		itoa(time->hour, &temp_1,10);
		time_[TIME_OFFSET+1] = temp_1;
		time_[TIME_OFFSET+2] = ':';
	}
	
	
	if(time->minute > 9){
		itoa(time->minute, temp ,10);
		time_[TIME_OFFSET+3] = temp[0];
		time_[TIME_OFFSET+4] = temp[1];
		time_[TIME_OFFSET+5] = ':';
	}else{
		time_[TIME_OFFSET+3] = '0';	
		itoa(time->minute, &temp_1,10);
		time_[TIME_OFFSET+4] = temp_1;
		time_[TIME_OFFSET+5] = ':';
	}
	
	
	if(time->second > 9){
		itoa(time->second, temp ,10);
		time_[TIME_OFFSET+6] = temp[0];
		time_[TIME_OFFSET+7] = temp[1];
	}else{
		time_[TIME_OFFSET+6] = '0';
		itoa(time->second, &temp_1,10);
		time_[TIME_OFFSET+7] = temp_1;
	}
	
	if(time->day > 9){
		itoa(time->day, temp ,10);
		date[DAY_OFFSET] = temp[0];
		date[DAY_OFFSET + 1] = temp[1];
	}else{
		date[DAY_OFFSET] = '0';
		itoa(time->day, &temp_1,10);
		date[DAY_OFFSET + 1] = temp_1;
	}
	char years[4];
	itoa(time->year, years ,10);
	date[YEAR_OFFSET] = years[0];
	date[YEAR_OFFSET + 1] = years[1];
	date[YEAR_OFFSET + 2] = years[2];
	date[YEAR_OFFSET + 3] = years[3];
	
	
}

