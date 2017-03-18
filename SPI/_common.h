
#ifndef __COMMON_
#define __COMMON_

#define LOW 0
#define HIGH 1
#define _min(a,b) ((a<b)?(a):(b))



typedef unsigned char uint8_t;
typedef unsigned char uchar;


typedef enum MEGA_PORT{
	PB,
	PC,
	PD
}MEGA_PORT;

typedef enum MEGA_PIN{
	P0 = 0,
	P1,
	P2,
	P3,
	P4,
	P5,
	P6,
	P7
}MEGA_PIN;
typedef struct PIN_ADDR{
	MEGA_PORT port;
	MEGA_PIN pin;
}PIN_ADDR;

typedef enum BOOL_{
	TRUE,
	FALSE
}BOOL;
#endif
