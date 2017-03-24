
#include "number2char.h"

struct CharLen number2char(uint8_t number){
	struct CharLen result;
	int i = 0;
	if(number > 99){
		result.len = 3;
	}else if(number > 9){
		result.len = 2;
	}else{
		result.len = 1;
	}
	i = result.len;
	result.header = (char *)malloc(i+1); // 4 bytes
	result.header[i] = '\\';
	i--;
	while(i >= 0){
		result.header[i] = (number % 10) + ASCII_OFFSET;
		number = number / 10;
		i--;
	}
	return result;
}
