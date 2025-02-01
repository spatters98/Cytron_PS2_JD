/****************************************
* Test program illustrating different   *
* methods of accessing PS2 controller   *
* keys                                  *
****************************************/
#include <Cytron_PS2_JD.h>

// create controller object with serial connection on pins 2&3
Cytron_PS2Shield ps2(2,3); 
#define CYCLETIME 2000  // mS between loop iterations

PS2key triangle(PS2_TRIANGLE);  // only used for key-based
PS2key squarex(PS2_SQUARE);      // query and fetch

void setup() {
 Serial.begin(9600);    // serial interface for console
 while(!Serial) {};     // wait for serial to start
 Serial.println("Started monitor serial");
 ps2.begin(9600);       // start up shield
 Serial.println("Attached controller");
  ps2.pushkey(triangle);
  ps2.pushkey(squarex);

 delay(1000);
}

void loop() {
    // legacy access using readButton()
    Serial.print("read cross:  ");
    Serial.println(ps2.readButton(PS2_CROSS));
    
    // direct query to shield using keyname
    int circle;
    ps2.query(PS2_CIRCLE, circle);
    Serial.print("Query circle: ");
    Serial.println(circle);

    // direct query to shield using PS2key object
    ps2.query(triangle);  
    Serial.print("Query triangle: ");
    Serial.print(triangle.value);
    Serial.print(" , ");
    Serial.println(triangle.changed);

    // make a query list
    // ps2.clearkeys();
  
    ps2.query();
    Serial.print("\nQuery triangle: ");
    Serial.print(triangle.value);
    Serial.print(" , ");
    Serial.println(triangle.changed);
    Serial.print("Query square: ");
    Serial.print(squarex.value);
    Serial.print(" , ");
    Serial.println(squarex.changed);

    // break out of loop
    if(ps2.readButton(PS2_SELECT) == 0) {
      exit(0); 
    }
    Serial.println("**********************\n");
    delay(CYCLETIME);
}
