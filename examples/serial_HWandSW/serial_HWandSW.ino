// Test code to diagnose servo palsy

#include <Servo.h>
#include <Cytron_PS2_JD.h>
//#define HWSERIAL  // flag for invoking hardware serial

#define SERVOPIN 7
#define SWUPDATETIME 250  // update limit for SW serial
#define HWUPDATETIME 50   // update limit for HW serial
Servo myServo;

#ifdef HWSERIAL
Cytron_PS2Shield ps2(0,1); 
// dummy function since we cannot print to serial monitor
printDiagnostic(String str0, int value = 0, String str1="") {
  return;
}
#else
Cytron_PS2Shield ps2(2,3);
// convenience function for print to the monitor
printDiagnostic(String str0, int value = 0, String str1="") {
  Serial.print(str0);
  Serial.print(value);
  Serial.println(str1);
  return;
}
#endif


// create some key definitions for testing fetch and query
// These need to be global so they can be use both loop() and setup()
// This permits the variables in the code to have descriptive names and
// allows easy change of the key bindings
PS2key triangle(PS2_TRIANGLE,ps2);        
PS2key square1(PS2_SQUARE, ps2); 
PS2key cross(PS2_CROSS, ps2);
PS2key circle(PS2_CIRCLE, ps2);          
PS2key joyRightY(PS2_JOYSTICK_RIGHT_Y_AXIS, ps2);
PS2key joyRightX(PS2_JOYSTICK_RIGHT_X_AXIS, ps2);
PS2key joyLeftY(PS2_JOYSTICK_LEFT_Y_AXIS, ps2);
PS2key joyLeftX(PS2_JOYSTICK_LEFT_X_AXIS, ps2);
PS2key up(PS2_UP,ps2);
PS2key down(PS2_DOWN, ps2);



void setup() {
#ifdef HWSERIAL
  ps2.setUpdateTime(HWUPDATETIME);
#else
  ps2.setUpdateTime(SWUPDATETIME);
  Serial.begin(9600);
  while(!Serial){};
  Serial.println("Started Monitor serial");
#endif
  
  myServo.attach(SERVOPIN);
  ps2.begin(9600);       // start up shield
  printDiagnostic("Attached controller");

  delay(500);
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(45);
}

void loop() {
  ps2.query();
  if(triangle.clicked()) {
    myServo.write(90);
    printDiagnostic("Going to ", 90);
  }
  if(cross.clicked()) myServo.write(45);
  if(circle.clicked()) myServo.write(0);
}
