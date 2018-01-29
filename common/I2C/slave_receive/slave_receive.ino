// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
   Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}
int x = 0;
void requestEvent() {
  Wire.write("hello "); // respond with message of 6 bytes

}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while ( Wire.available()) { // loop through all but the last
    int c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    Serial.print(" ");
   Serial.print((char)c);
   Serial.print('\n');
  }
      // print the integer
}
