/*
Original written by:
            Cytron Technologies

Modified:
  29/06/15  Idris, Cytron Technologies    - Point to IDE SoftwareSerial
                                          - Restructure the code style to follow standard Arduino library

Modified:
  30/01/2025 S. Patterson
*/
#include <PostNeoSWSerial.h>      // software serial for connecting to Cytron_PS2Shield
#include <ArduinoSTL.h>           // standard C++ template library support for vector

#ifndef Cytron_PS2Shield_h
#define Cytron_PS2Shield_h

#include "Arduino.h"

// Arduino Leonardo
#if defined (__AVR_ATmega32U4__)
  #define Serial Serial1
#else 
  #define Serial Serial
#endif

// Define PS2 button to number
//   The resulting enum integer value corresponds to the
//   command code sent to the Cytron board to retrieve the state
//   DO NOT CHANGE THE ORDER of the enums
enum {
  // Digital button
  PS2_SELECT = 0,
  PS2_JOYSTICK_LEFT,
  PS2_JOYSTICK_RIGHT,
  PS2_START,
  PS2_UP,
  PS2_RIGHT,
  PS2_DOWN,
  PS2_LEFT,
  PS2_LEFT_2,
  PS2_RIGHT_2,
  PS2_LEFT_1,
  PS2_RIGHT_1,
  PS2_TRIANGLE,
  PS2_CIRCLE,
  PS2_CROSS,
  PS2_SQUARE,
  // Analog button
  PS2_JOYSTICK_LEFT_X_AXIS,
  PS2_JOYSTICK_RIGHT_X_AXIS,
  PS2_JOYSTICK_RIGHT_Y_AXIS,
  PS2_JOYSTICK_LEFT_UP,
  PS2_JOYSTICK_LEFT_DOWN,
  PS2_JOYSTICK_LEFT_LEFT,
  PS2_JOYSTICK_LEFT_RIGHT,
  PS2_JOYSTICK_RIGHT_UP,
  PS2_JOYSTICK_RIGHT_DOWN,
  PS2_JOYSTICK_RIGHT_LEFT,
  PS2_JOYSTICK_RIGHT_RIGHT,
  // Check connection status
  PS2_CONNECTION_STATUS,
  // Control motor vibrarion
  PS2_MOTOR_1,
  PS2_MOTOR_2,
  // Read all button
  PS2_BUTTON_JOYSTICK
};

class PS2key {
  public:
    PS2key(uint8_t keyname);
    uint8_t name();
    bool changed;     // flag for new value
    int value;      // variable to carry result     
  protected:
    uint8_t kname;   // the name of key on controller
};

class Cytron_PS2Shield
{
  public:
	  boolean SERIAL_ERR;
	  uint8_t ps_data[6];
    uint8_t _txpin, _rxpin;

    // Software Serial
    Cytron_PS2Shield(uint8_t rxpin, uint8_t txpin);
    // Hardware Serial
    Cytron_PS2Shield();
  
    void begin(uint32_t baudrate);
    uint8_t readButton(uint8_t key);
    boolean readAllButton();
    void vibrate(uint8_t motor, uint8_t value);
    void reset(uint8_t reset);
    // ---- begin PS2Link functions ----
    void setUpdateTime(int mS);   // change the minimum interval between queries
    bool query(uint8_t key, int &value); 
    bool query(PS2key &key);
    bool query();
    void pushkey(PS2key &key);  // add a key ptr to the query list
    void clearkeys();           // clear the query list
  protected:
    boolean hardwareSerial;
#ifdef PostNeoSWSerial_h
    PostNeoSWSerial *PS2Serial;
#else
    SoftwareSerial *PS2Serial;
#endif
    void write(uint8_t data);
    uint8_t read(void);
    
  private:    // added for PS2Link functions
    unsigned long int tlast;      // update timer
    int updatems=10;              // wait time for updates in mS
    std::vector<PS2key*> keylist;  // vector to hold the list of keys to query
    bool checkupdatetime();       // return true if it is a valid update time
};

#endif
