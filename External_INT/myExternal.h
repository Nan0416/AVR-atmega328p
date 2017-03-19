#ifndef _MY_EXTERNAL_
#define _MY_EXTERNAL_

#include "_common.h"
typedef enum EXT_INT{
	EXT_INT_0 = 0x03,
	EXT_INT_1 = 0x0C
}EXT_INT;

typedef enum EXT_MODE{
	RISING_EDGE = 0x0F,
	FALLING_EDGE = 0x0A,
	LOW_LEVEL = 0x00,
	EDGES = 0x05
}EXT_MODE;

typedef struct myEXT{
	EXT_INT int_no;
	EXT_MODE int_mode;
	BOOL enabled;
}myEXT;



#define PORT_DIRECTION_INT0 0xFB
#define PORT_DIRECTION_INT1 0xF7
#define PORT_DIRECTION_INT 0xF3



#define ENABLE_INT0 0x01
#define ENABLE_INT1 0x02
#define ENABLE_INT 0x03


void init_ext(myEXT * ext);
void setup_ext(myEXT * ext);
void config_ext_no(myEXT * ext, EXT_INT int_no);
void config_ext_mode(myEXT * ext, EXT_MODE mode);
void config_ext_enable(myEXT * ext, BOOL enable);
void clear_ext_irq_flag(myEXT * ext);

#endif //
