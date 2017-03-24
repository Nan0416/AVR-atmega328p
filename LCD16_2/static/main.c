#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "number2char.h"

int main(){
	setupLCD();
	char * info = "QIN NAN";
//	char * info2 = "QINNNN";
	struct CharLen charCon;
	
	showData(info, 7, LINE_2);
	_delay_ms(1000);
	clear();
	charCon = number2char(100);
	showData(charCon.header, charCon.len, LINE_2);
	_delay_ms(1000);
	clear();
	charCon = number2char(99);
	showData(charCon.header, charCon.len, LINE_2);
	_delay_ms(1000);
	clear();
	charCon = number2char(3);
	showData(charCon.header, charCon.len, LINE_2);
	while(1);
	return 0;
}
