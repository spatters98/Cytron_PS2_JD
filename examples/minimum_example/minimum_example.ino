/****************************************
* Example program illustrating a simple *
* use of PS2 controller   SQUARE button *
* to control the LED on the Arduino     *
****************************************/
#include <Cytron_PS2_JD.h>

#define LED 13    // the pin number of the led
// create controller object with serial connection on pins 2&3
Cytron_PS2Shield ps2(2,3); 

// create the led key and push on key list    
PS2key led(PS2_SQUARE, ps2); 

void setup() {
  ps2.begin(9600);       // start up shield
  pinMode(LED,OUTPUT);   // set up the output pin for the LED
}

void loop() {
  ps2.query();          // query the key list
  digitalWrite(LED,(led.value)? LOW:HIGH);
}
