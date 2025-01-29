# Cytron_PS2_JD
Cytron PS2 Shield Arduino IDE library
This code builds on the Cytron_PS2Shield library originally written by Cytron and updated by Idris Sainal Abidin. This version addresses the issue of wait time of the CPU during serial communcation between the Arduino and the Cytron shield. In particular, it introduces a get() method that fetches a button value from memory, allowing a single updateAll() to be issued once rather than a series of individual read() for each button.

The library also allows for automatic insertion of new values into main program variables using a PS2key type.

This library calls the **my_STL** library to provide features of the C++ Standard Template Library. This can cause incompatibilities if another, different, STL library is used in the main program.

This library will create a software serial port for communication with the Cytron shield. Since there are multiple software serial port libraries, one is not included here. The inclusion of a software serial port library must be made prior to including this library. The recommended software serial port library is ++PostNeoSWSerial** by Hexaedron. This library has been tested with version 1.0.5 of that library. Note that only 9600 baud is supported if this library is used. Alternatively, the included **SoftwareSerial** library can be used.  
