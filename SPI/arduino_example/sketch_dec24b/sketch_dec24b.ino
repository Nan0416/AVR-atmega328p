


#include <stdlib.h>
#include <SPI.h>


#define SCK_PIN  13
#define MISO_PIN 12
#define MOSI_PIN 11
#define SS_PIN   10

int i = 0;

char k[20], v[20];
int valid = 0;

void led(void){
  DDRC |= 0x01;
  PORTC |= 0x01;
  _delay_ms(100);
  PORTC &= 0xFE;
  _delay_ms(200);
}

void setup (void)
{
  
  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  SPI.attachInterrupt();
}

ISR (SPI_STC_vect)
{
  char c = SPDR;
  if (c == '\n')
  {
    led();
    i = 0;
  }
  else if (c != '\n')
  {
    k[i] = c;
    i++;
  }
}

void loop (void)
{
  while (1);
}


