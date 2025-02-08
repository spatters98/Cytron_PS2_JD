/**
 * @file Cytron_PS2_JD.h
 * @brief Header file for PS2key and Cytron_PS2Shield classes
 * @author S Patterson
 * @author Idris, Cytron Technologies
 * @version 0.2
 * @date Feb 2025
 * 
 * This is the header file for the PS2key and Cytron_PS2Shield classes.
 * The Cytron_PS2Shield class was originally written by Idris in 2015. 
 * The PS2key class and some modifications to that class were made by S Patterson.
 * 
 */


#ifndef Cytron_PS2_JD_h
#define Cytron_PS2_JD_h

#include <PostNeoSWSerial.h>      // software serial for connecting to Cytron_PS2Shield
#include <ArduinoSTL.h>           // standard C++ template library support for vector
#include <stdint.h>     // not needed for Arduino IDE, but surpresses errors in KDevelop

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
  PS2_LEFT, //7
  PS2_LEFT_2,
  PS2_RIGHT_2,
  PS2_LEFT_1,
  PS2_RIGHT_1,
  PS2_TRIANGLE,
  PS2_CIRCLE,
  PS2_CROSS,
  PS2_SQUARE,   //15
  // Analog button
  PS2_JOYSTICK_LEFT_X_AXIS,
  PS2_JOYSTICK_LEFT_Y_AXIS,
  PS2_JOYSTICK_RIGHT_X_AXIS,
  PS2_JOYSTICK_RIGHT_Y_AXIS,
  PS2_JOYSTICK_LEFT_UP,
  PS2_JOYSTICK_LEFT_DOWN,
  PS2_JOYSTICK_LEFT_LEFT,
  PS2_JOYSTICK_LEFT_RIGHT,//23
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
  PS2_BUTTON_JOYSTICK   //31
};

class  Cytron_PS2Shield;  // forward declaration

union Packet {            // location for data from readAllButton
  uint64_t value;
  uint8_t byte[8];        // note that the Cytron shield only uses 6 bytes
  uint16_t switches[4];   // only first word will be used for digital switches
};

/**
 * @class PS2key
 * @brief Represents a key on a PS2 controller.
 */
class PS2key {
  public:
     /**
     * @brief Constructs a PS2key object.
     * 
     * @param keyname The identifier for the key.
     */
    PS2key(uint8_t keyname);
    
    /**
     * @brief Constructs a PS2key object and adds it to a Cytron_PS2Shield object
     * key list.
     * 
     * @param keyname The identifier for the key.
     * @param ps Reference to a Cytron_PS2Shield object to which this key will be added.
     */
    PS2key(uint8_t keyname, Cytron_PS2Shield &ps);
    uint8_t name();   // return the uint8_t key name
    bool changed;     // flag for new value
    int value;        // variable to carry result  
    bool clicked();   // returns true and clears changed if key is just changed
    bool pressed();   // return true if key is presently pressed (zero value)
    bool released();  // return true and clear changed if key has just been released
  protected:
    uint8_t kname;   // the name of key on controller
};

/**
 * @class Cytron_PS2Shield
 * @brief A class to interface with the Cytron PS2 Shield.
 */
class Cytron_PS2Shield
{
  public:
    bool SERIAL_ERR;
    uint8_t _txpin, _rxpin;

    // Software Serial
    /**
    * @brief Constructs a Cytron_PS2Shield object with specified RX and TX pins.
    * 
    * If the pins are 0 and 1, then the code assumes it is a hardware
    * serial connection. Otherwise it uses the PostNeoSWSerial library
    * to establish a software serial connection on the specified pins.
    * If this library is to be used with other than an Uno R3 and hardware
    * serial is desired on pins other than 0 and 1, the .cpp code must be modified.
    * 
    * @param rxpin The receive pin number.
    * @param txpin The transmit pin number.
    */
    Cytron_PS2Shield(uint8_t rxpin, uint8_t txpin);
    
    /**
    * @brief Constructs a Cytron_PS2Shield object with default RX (2) and TX (3) pins.
    * 
    * Software serial is assumbed and created using the PostNeoSWSerial library.
    */
    Cytron_PS2Shield();
    
    /**
    * @brief Initializes the PS2 Shield communication.
    * 
    * This function sets up the serial communication with the PS2 Shield.
    * It determines whether to use hardware or software serial based on the pin configuration. For software serial the baud rate must be 9600.
    * 
    * @param baudrate The baud rate for serial communication.
    * 
    * @note If RX is 0 and TX is 1, hardware serial is used. Otherwise, software serial is employed.
    * @note Pin A1 is set as OUTPUT and pulled HIGH as part of the initialization.
    */
    void begin(uint32_t baudrate = 9600);
    uint8_t readButton(uint8_t key);
    bool readAllButton();
    void vibrate(uint8_t motor, uint8_t value);
    void reset(uint8_t reset);
    // ---- begin PS2Link functions ----
    void setUpdateTime(int mS);   // change the minimum interval between queries
    bool query(uint8_t key, int &value); // query a particular key by name
    bool query(PS2key &key);    // query a particular key
    bool query();               // query each of the keys in key list
    void pushkey(PS2key &key);  // add a key ptr to the query key list
    void clearkeys();           // clear the query list
    bool queryAll();            // query all of the controller with packet response
    void fetch(uint8_t key, int &value); // fetch a key by name
    void fetch(PS2key &key);    // fetch a key from buffer
    void fetch();               // fetch all the keys in key list
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
    Packet ps_data;               // return storage for readAllButton
    uint8_t switchValues[24];     // array to store values decoded from packet
    int keyBitMap[16] = {         // map from packet to key names
      PS2_SELECT,
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
      PS2_SQUARE
    };
};

#endif
