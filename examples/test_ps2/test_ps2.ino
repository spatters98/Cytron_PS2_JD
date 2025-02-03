/****************************************
* Test program illustrating different   *
* methods of accessing PS2 controller   *
* keys                                  *
****************************************/
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
PS2key gripOn(PS2_TRIANGLE);        
PS2key gripOff(PS2_SQUARE); 
// calling the constructor with the second argument
// causes the variable to be added to the key list automatically
PS2key craneUp(PS2_CROSS, ps2);
PS2key craneDown(PS2_CIRCLE, ps2);          
PS2key motorSpeed(PS2_JOYSTICK_RIGHT_Y_AXIS, ps2);

void setup() {
  Serial.begin(9600);    // serial interface for console
  while(!Serial) {};     // wait for serial to start
  Serial.println("Started monitor serial");
  ps2.begin(9600);       // start up shield
  Serial.println("Attached controller");

 // add items to key list that were not automatically
 // created at the time of declaration
  ps2.pushkey(gripOn);
  ps2.pushkey(gripOff);

  delay(1000);  // short pause before loop starts - not necessary
}

void loop() {
    unsigned long int tstart, period;   // variables used for timing measurements
    Serial.println("---------------------------");  // mark start of iteration

        // legacy access using readButton()
    printDiagnostic("value of cross button = ", ps2.readButton(PS2_CROSS));
    
    // direct query to shield using keyname
    int circle;
    ps2.query(PS2_CIRCLE, circle);
    printDiagnostic("Query circle: ", circle, "\n");
    
    // direct query to shield using PS2key object
    ps2.query(gripOn);  
    printDiagnostic("Query gripOn (TRIANGLE): ", gripOn.value);
    printDiagnostic("Is this a new value> ", gripOn.changed, "\n");

    // use the query list created in setup()
    tstart = millis();
    ps2.query();    // this queries all key list entries successively
    period = millis()-tstart;
    printDiagnostic("query() time = ", period, " mS");
    printDiagnostic("query gripOn: ", gripOn.value);
    printDiagnostic("gripOn changed? ", gripOn.changed);
    printDiagnostic("query gripOff:", gripOff.value);
    printDiagnostic("gripOff changed? ", gripOff.changed);
    printDiagnostic("query motorSpeed: ", motorSpeed.value);
    printDiagnostic("motorspeed changed? ", motorSpeed.changed, "\n");
        
    // query all values at once
    tstart = millis();
    ps2.queryAll();
    period = millis()-tstart;
    printDiagnostic("queryAll() time = ", period, " mS");
    
    // fetch directly with key name
    int temp;
    ps2.fetch(PS2_UP, temp);
    printDiagnostic("value of PS2_UP = ",temp);

    // fetch a key
    ps2.fetch(gripOn);
    printDiagnostic("single key fetch gripOn = ",gripOn.value);
    
    ps2.fetch();  // fetch everything in key list
    printDiagnostic("fetch gripOn: ", gripOn.value);
    printDiagnostic("gripOn changed? ", gripOn.changed);
    printDiagnostic("fetch gripOff:", gripOff.value);
    printDiagnostic("gripOff changed? ", gripOff.changed);
    printDiagnostic("fetch craneUp: ", craneUp.value);
    printDiagnostic("craneUP changed? ", craneUp.changed);
    printDiagnostic("fetch craneDown: ", craneDown.value);
    printDiagnostic("craneDown changed? ", craneDown.changed);
    printDiagnostic("fetch motorSpeed: ", motorSpeed.value);
    printDiagnostic("motorspeed changed? ", motorSpeed.changed, "\n");

    Serial.println("**************************");  // done with this iteration of loop

   // break out of loop if desired
    if(ps2.readButton(PS2_SELECT) == 0) {
      exit(0); 
    }
    delay(CYCLETIME);   // make sure the loop runs slow enough to see whats happening
}
