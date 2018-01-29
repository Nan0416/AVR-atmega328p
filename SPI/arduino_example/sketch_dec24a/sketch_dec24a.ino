#include <stdlib.h>
#include <SPI.h>


int i = 0;
char inData[64];
char k[30];
char inChar;
int valid = 0;
int index = 0;

#define SCK_PIN  13
#define MISO_PIN 12
#define MOSI_PIN 11
#define SS_PIN   10

void setup()

{

  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);

  digitalWrite(SS_PIN, HIGH);
  SPI.begin();


}

void transferAndWait(char what)
{
  SPI.transfer(what);
  delayMicroseconds(20);
}
void led(void){
  DDRC |= 0x01;
  PORTC |= 0x01;
  _delay_ms(100);
  PORTC &= 0xFE;
  _delay_ms(200);
}

void send(char *p)
{
  char c;
  while (*p)
  {
    c = *p++;
    transferAndWait(c);
    delay(200);
  }
}


void loop()
{
  digitalWrite(SS_PIN, LOW);
  send("Hello World...\n");
  led();
  delay(100);
}
