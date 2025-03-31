/**
 * @file PS2Link.cpp
 * @brief File with PS2key class functions and additions to Cytron_PS2Shield class.
 * @author S Patterson
 * @date Feb 2025
 * @version 0.2
 * 
 * This file contains all of the PS2key class code and additions to the modified
 * original Cytron code contained in Cytron_PS2_JD.cpp. That file must be included
 * when building the Arduino library.
 */

#define DEBUG

#include "Cytron_PS2_JD.h"

PS2key::PS2key(uint8_t keyname) : kname(keyname){}

PS2key::PS2key(uint8_t keyname, Cytron_PS2Shield &ps) : kname(keyname) {
    ps.pushkey(*this);
}

uint8_t PS2key::name() { return kname;}

/**
 * @brief Checks if the key was just clicked (pressed down).
 * 
 * @return true if the key was just pressed down, false otherwise.
 */
bool PS2key::clicked() {
    if(value == 0 && changed) { // key pressed and it is new value
        changed = false;        // clear the changed flag
        return true;
    }
    else return false;
}

/**
 * @brief Checks if the key was just released.
 * 
 * @return true if the key was just released, false otherwise.
 */
bool PS2key::released() {
    if(value == 1 && changed) {
        changed = false;
        return true;
    }
    else return false;
}

/**
 * @brief Checks if the key is currently pressed.
 * 
 * @return true if the key is currently pressed, false otherwise.
 */
bool PS2key::pressed() {return (value == 0);}

/**
 * @brief Checks if time is OK to update based on the set update interval.
 * 
 * This function is used to determine that at least the specified update
 * time has elapsed since the last query to the Cytron shield. It is used
 * to prevent shield queries from taking an excessive fraction of available
 * CPU time.
 * 
 * @return true if it's OK to update, false otherwise.
 */
bool Cytron_PS2Shield::checkupdatetime() {
    if((millis() - tlast) < updatems) return false;
        else {
            tlast += updatems;  // set the timer for next interval
            return true;
        }
}

/**
 * @brief Queries the Cytron shield for the state of a specific key.
 * 
 * @param key The key to query.
 * @param value Reference to store the key's value.
 * @return true if the query was performed, false if it's not time to update yet.
 */
bool Cytron_PS2Shield::query(uint8_t key, int &value) {
    if (!checkupdatetime()) return false;
    value = readButton(key);
    return true;
}

/**
 * @brief Queries the Cytron shield for the state of a PS2key object.
 * 
 * @param key Reference to the PS2key object to update.
 * @return true if the query was performed, false if it's not time to update yet.
 */
bool Cytron_PS2Shield::query(PS2key &key) {
    if (!checkupdatetime()) {
        key.changed = false;
        return false;
    }
    int newvalue = readButton(key.name());
    key.changed = (newvalue != key.value);
    key.value = newvalue;
    return true;
}

/**
 * @brief Successively queries the Cytron shield for the state of all keys in the keylist.
 * 
 * Since this function communicates with the shield for each key on the key list,
 * the amount of CPU time used is proportional to the number of keys on the key
 * list. This can be quicker than using a queryAll() when there are fewer than four
 * keys on the list. For longer key lists *queryAll(); fetch();* should be used.
 * 
 * @return true if the query was performed, false if it's not time to update yet.
 */
bool Cytron_PS2Shield::query() {
    if (!checkupdatetime()) return false;
    for(auto& keyptr : keylist) {
    uint8_t keyval = readButton(keyptr->name());
    keyptr->changed = (keyval != keyptr->value);
    keyptr->value = keyval;
    }
    return true;
}

/**
 * @brief Queries the state of all buttons and joysticks from the PS2 shield.
 * 
 * This function updates the internal state of all buttons and joystick axes. 
 * The result is stored in a buffer on the Arduino, but not assigned any keys.
 * *fetch()* should be used to update keys from the buffer.
 * 
 * @return true if the query was performed, false if it's not time to update yet.
 */
bool Cytron_PS2Shield::queryAll()   // get packet from shield
{
    if (!checkupdatetime()) return false;
    else {
        readAllButton();
        // first get the digital keys
        const uint16_t mask = 1;
        uint16_t tempdata = ps_data.switches[0];
        for (int i = 0 ; i<16 ; i++) {
            switchValues[keyBitMap[i]] = tempdata & mask;
            tempdata >>= 1;
        }
        // then get the joystic values
        switchValues[PS2_JOYSTICK_LEFT_X_AXIS] = ps_data.byte[2];
        switchValues[PS2_JOYSTICK_LEFT_Y_AXIS] = ps_data.byte[3];
        switchValues[PS2_JOYSTICK_RIGHT_X_AXIS] = ps_data.byte[4];
        switchValues[PS2_JOYSTICK_RIGHT_Y_AXIS] = ps_data.byte[5];
        return true;
    }
}

/**
 * @brief Updates a specific controller key from the buffer on the Arduino
 * 
 * @param key The controller key to fetch.
 * @param value Reference to store the key's value.
 */
void Cytron_PS2Shield::fetch(uint8_t key, int& value)
{
    value = switchValues[key];
    return ;
}

/**
 * @brief Fetches the current value for a single PS2 key.
 * 
 * This function updates the value and changed status of a given PS2 key
 * from the buffer on the Arduino. It does *not* query the Cytron shield.
 * 
 * @param key Reference to a PS2key object to be updated.
 */
void Cytron_PS2Shield::fetch(PS2key &key)
{
    int keyval = switchValues[key.name()];
    key.changed = keyval != key.value;
    key.value = keyval;
    return;
}

/**
 * @brief Fetches the current values for all PS2 keys in the key list.
 * 
 * This function iterates through all keys in the keylist and updates their
 * values from the buffer on the Arduino. It does *not* query the Cytron shield.
 * As a result it is very fast and does not contribut significantly to loop time.
 */
void Cytron_PS2Shield::fetch()
{
    for(auto &keyptr : keylist) {
        fetch(*keyptr);
    }
    return;
}

/**
 * @brief Clears all keys from the keylist.
 * 
 * This function removes all keys from the key list.
 * This would only be of interest for debug/development
 */
void Cytron_PS2Shield::clearkeys() {
    keylist.clear();
}

/**
 * @brief Adds a key to the key list.
 * 
 * This function adds a PS2 key to the internal keylist for future querying.
 * 
 * @param key Reference to a PS2key object to be added to the keylist.
 */
void Cytron_PS2Shield::pushkey(PS2key &key) {  // add a key to the query list
      keylist.push_back(&key);
}

/**
 * @brief Sets the update time interval.
 * 
 * This function sets the *minimum* time interval (in milliseconds) between
 * calls to the Cytron shield.
 * 
 * @param mS The interval in milliseconds.
 */
void Cytron_PS2Shield::setUpdateTime(int mS) {
    updatems = mS;
}
