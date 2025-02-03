/****************************************
* Example program illustrating a simple *
* use of PS2 controller right joystick  *
* to control a servo on pin 8           *
****************************************/
#include <Cytron_PS2_JD.h>
#include <Servo.h>

#define SERVOPIN 8      // the pin number of the servo
// create controller object with serial connection on pins 2&3
Cytron_PS2Shield ps2(2,3); 
Servo myServo;

// create the led key and push on key list    
PS2key joystick_y(PS2_JOYSTICK_RIGHT_Y_AXIS, ps2); 

void setup() {
  ps2.begin(9600);                  // start up shield
  myServo.attach(SERVOPIN);         // set up the output pin for the servo
  myServo.write(0); delay(1000);    // verify servo is good on startup
  myServo.write(90); delay(1000);
  myServo.write(180); delay(1000);
  myServo.write(90);
}

void loop() {
  static int current_position;              // value of last position
  ps2.query();                              // query the key list 
  int position = (joystick_y.value*45)/64;  // map joystick to servo range 
  if (position != current_position) {       // only if data has changed
    myServo.write(position);                // move servo with new data
    current_position = position;            // update current position
  }
}
