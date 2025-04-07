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

---The following *fetch()* discussion does not work reliably ---
PS2key objects can be updated either by directly querying the shield or by fetching from a buffer that has been previously filled by a *queryAll()* which downloads the data for all keys from the shield into the buffer. *query()* successively queries the shield for each key on the key list. *fetch()* updates all keys on the key list from the buffer. Each individual key query. An individual key query takes ~2mS; querying all of the keys results in a 6-byte transfer and takes ~7-8mS. For most applications it will be easiest to *queryAll()* and then *fetch()* to update the key list.
--------------------------------------------

The original *readButton()* function immediately initiated communication with the shield. If this was repeatedly called in a loop it could result in distracting the CPU sufficiently to cause conflicts with other library functions such as those for servo control. This was often addressed by placing a *delay()* at the end of the loop. But this is bad practice in that it slows down the loop unneccessarily. The *query()* functions have a built in timer that they only query the shield if the previous communication has occured a specified time before. Otherwise it returns and the values in the PS2key object are unchanged. This allows the *query()* functions to be called in the loop safely regardless of the loop timing. 
