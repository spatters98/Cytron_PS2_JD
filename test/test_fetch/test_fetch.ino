/*********************************************
* Test program illustrating use of fetch()   *
* to access PS2 controller keys              *
*                                            *
*********************************************/
#include <Cytron_PS2_JD.h>

// create controller object with serial connection on pins 2&3
Cytron_PS2Shield ps2(2,3); 
#define CYCLETIME 4000  // mS between loop iterations

// convenience function for print to the monitor
printDiagnostic(String str0, int value,String str1=""){
  Serial.print(str0);
  Serial.print(value);
  Serial.println(str1);
  return;
}

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
  Serial.begin(9600);    // serial interface for console
  while(!Serial) {};     // wait for serial to start
  Serial.println("Started monitor serial");
  ps2.begin(9600);       // start up shield
  Serial.println("Attached controller");
  delay(1000);  // short pause before loop starts - not necessary
}

void loop() {
    Serial.println("---------------------------");  // mark start of iteration
    ps2.queryAll();
    //ps2.fetch(triangle);
    //ps2.fetch(square1);
    //ps2.queryAll();
    ps2.fetch();  // fetch everything in key list
    printDiagnostic("fetch triangle: ", triangle.value);
    printDiagnostic("fetch square: ", square1.value);
    printDiagnostic("fetch cross: ", cross.value);
    printDiagnostic("fetch circle: ", circle.value);
    printDiagnostic("fetch joyRightX: ", joyRightX.value);
    printDiagnostic("fetch joyRightY: ", joyRightY.value);
    printDiagnostic("fetch joyLeftX: ", joyLeftX.value);
    printDiagnostic("fetch joyLeftY: ", joyLeftY.value);
    printDiagnostic("UP",up.value);
    printDiagnostic("DOWN", down.value);
    Serial.println("**************************");  // done with this iteration of loop

   // break out of loop if desired
    if(ps2.readButton(PS2_SELECT) == 0) {
      exit(0); 
    }
    delay(CYCLETIME);   // make sure the loop runs slow enough to see whats happening
}