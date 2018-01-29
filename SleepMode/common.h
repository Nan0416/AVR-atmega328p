
#ifndef __COMMON_
#define __COMMON_

#define LOW 0
#define HIGH 1
#define _min(a,b) ((a<b)?(a):(b))
#ifndef NULL
#define NULL ( (void *) 0)
#endif

#ifndef NOP
#define NOP 0xFF
#endif
typedef unsigned char uint8_t;
typedef unsigned char uchar;

typedef uint16_t size_t;
typedef enum MEGA_PORT{
	PB,
	PC,
	PD,
	UNDEFINED
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

typedef enum pin_type{
	port,
	pin
}pin_type;

typedef enum BOOL_{
	TRUE,
	FALSE
}BOOL;
#endif
