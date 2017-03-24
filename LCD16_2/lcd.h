#ifndef __LCD_
#define __LCD_

#define LINE_1 1
#define LINE_2 2
void setupLCD(void);
void _writeInCMD(uint8_t CMD);
void _writeInDATA(uint8_t DATA);
void showData(char * data, uint8_t len, uint8_t line);
void clear(void);
#endif //__LCD_








