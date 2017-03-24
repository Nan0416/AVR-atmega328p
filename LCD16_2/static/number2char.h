#ifndef NUMBER2CHAR
#define NUMBER2CHAR
#include <stdlib.h>
#include <stdint.h>
#define ASCII_OFFSET 48
struct CharLen{
	char * header;
	uint8_t len;
};

struct CharLen number2char(uint8_t);
#endif // NUMBER2CHAR
