# Cytron_PS2_JD
Cytron PS2 Shield Arduino IDE library
This code builds on the Cytron_PS2Shield library originally written by Cytron and updated by Idris Sainal Abidin. This version addresses the issue of wait time of the CPU during serial communcation between the Arduino and the Cytron shield. In particular, it introduces a get() method that fetches a button value from memory, allowing a single updateAll() to be issued once rather than a series of individual read() for each button.

The library also allows for automatic insertion of new values into main program variables using a PS2key type.
