// placeholder example
#include<PostNeoSWSerial.h>
#include <Cytron_PS2_JD.h>

Cytron_PS2Shield ps2(2,3);  // controller object using pins 2 and 3

void setup() {
 Serial.begin(9600);    // serial interface for console
 
}

void loop() {
 String msg = "";
msg.concat(millis());
msg.concat("\t");
msg.concat(PS2.readButton(PS2_CROSS));
Serial.println(msg);
}
