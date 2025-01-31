// placeholder example
#include <Cytron_PS2_JD.h>

Cytron_PS2Shield ps2(2,3);  // controller object using pins 2 and 3

void setup() {
 Serial.begin(9600);    // serial interface for console
 while(!Serial) {};     // wait for serial to start
 Serial.println("Started serial");
 ps2.begin(9600);
 Serial.println("Attached controller");
 delay(5000);
 Serial.println("Still here");
 
}

void loop() {
  for(int i=0; i < 5; i++) {
    Serial.print("read cross:  ");
    String msg = "";
    msg.concat(millis());
    msg.concat("\t");
    msg.concat(ps2.readButton(PS2_CROSS));
    Serial.println(msg);

    delay(200);

    int circle;
    ps2.query(PS2_CIRCLE, circle);
    Serial.print("Query circle: ");
    Serial.println(circle);

    if(ps2.readButton(PS2_SELECT) == 0) {
      Serial.println("bye bye");
      exit(0);
    }
    delay(1000);
  }
  
}
