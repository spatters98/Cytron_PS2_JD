# Cytron_PS2_JD
Cytron PS2 Shield Arduino IDE library
This code builds on the Cytron_PS2Shield library originally written by Cytron and updated by Idris Sainal Abidin in 2015. This version addresses the issue of wait time of the CPU during serial communcation between the Arduino and the Cytron shield. In particular, it introduces a *fetch()* method that fetches a button value from memory, allowing a single *queryAll()* to be issued once rather than a series of individual *read()* for each button.

The library also allows for automatic insertion of new values into main program variables using a PS2key type.

This library calls the **my_STL** library to provide features of the C++ Standard Template Library. This can cause incompatibilities if another, different, STL library is used in the main program.

This library will create a software serial port for communication with the Cytron shield if it is instantiated with other than pins 0 and 1 used for communication. There are multiple software serial port libraries available. The one called in the header file is **PostNeoSWSerial** by Hexaedron. This library has been tested with version 1.0.5 of that library. Note that only 9600 baud is supported when this library is used. Alternatively, the Arduini IDE provided **SoftwareSerial** library can be used. To do this, edit the **#include <PostNeoSWSerial.h>** line in the header file and replace it with **SoftwareSerial.h**.  The PostNeoSWSerial library is used here by default to reduce the CPU load from communication to the Cytron shield.

The *readButton(uint8_t keyname)* and *readAllButton()* from the original library are retained. In addition, this library provides additional ways to access the shield in an object-oriented way and provides a degree of timing safety. The recommended access using this library is to create a PS2key object with name appropriate to its intended use (e.g. *PS2key closeGrip(PS2_TRIANGLE, ps2)*. Creating the PS2key object with the two-argument constructor, where the second argument references a Cytron_PS2Shield object automatically places the PS2key object on a key list which allows for simple updating of all keys on the list.

PS2key objects have the following methods:

    - name()      returns the uint8_t key name
    - value       int returned value from the shield
    - clicked()   bool value indicating that the key is newly pressed
    - released()  bool value indicating that the key is newly released
    - pressed()   bool value indicating that the key is currently pressed
    
It should be noted that *clicked()* and *released()* return a true value only the **first** time they are called after the action. In the case of values from the joysticks, *.value* contains the 8-bit value returned and *clicked()* is true if the current value is different than the previous one.


The original *readButton()* function immediately initiated communication with the shield. If this was repeatedly called in a loop it could result in distracting the CPU sufficiently to cause conflicts with other library functions such as those for servo control. This was often addressed by placing a *delay()* at the end of the loop. But this is bad practice in that it slows down the loop unneccessarily. The *query()* functions have a built in timer that they only query the shield if the previous communication has occured a specified time before. Otherwise it returns and the values in the PS2key object are unchanged. This allows the *query()* functions to be called in the loop safely regardless of the loop timing. 

When the Cytron shield is connected to the Arduino using software serial (something other than pins 0 and 1), the interrupts used by the timers are turned off for a brief period during the data transfer. This can corrupt a control pulse sent to a servo. If the shield is continuously interrogated, the effect is strong enough to keep the servo from being functional. If the shield is interrogated periodically, then the effect is reduced, but never entirely eliminated. The residual corruption is exhibited as a small occasional jitter about the commanded position; so-called "servo palsy." 

The minimum time between interrogations in milliseconds can be set by the member function *setUpdateTime(value)*. As the update time is increased, the observable jitter is decreased at the expense of reduced controller responsiveness. Jitter will also depend on the number of keys being queried, with a larger keylist resulting in more jitter. The default value for update time is 10ms, corresponding to 100 updates/second from the controller. Values up to 100ms should be acceptable in operation, and even larger values can be used for testing.

Jitter can be eliminated entirely by using the hardware serial interface on pins 0 and 1 of the Arduino Uno. However, using the hardware serial connection to the Cytron shield will prevent using the Serial Console of the Arduino IDE for program monitoring and debug. To use the hardware serial interface, the Cytron shield jumpers must be set accordingly and the ps2 object instantiated on pins 0 and 1 (i.e., *Cytron_PS2Shield ps2(0,1)*).
