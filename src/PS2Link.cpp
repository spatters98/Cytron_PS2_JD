/**************************************
*   Additions to Cytron_PS2Shield     *
*   to incorporate PS2Link functions  *
**************************************/
#define DEBUG

#include "Cytron_PS2_JD.h"

PS2key::PS2key(uint8_t keyname) : kname(keyname){}

PS2key::PS2key(uint8_t keyname, Cytron_PS2Shield &ps) : kname(keyname) {
    ps.pushkey(*this);
}

uint8_t PS2key::name() { return kname;}

bool PS2key::clicked() {
    if(value == 0 && changed) { // key pressed and it is new value
        changed = false;        // clear the changed flag
        return true;
    }
    else return false;
}

bool PS2key::released() {
    if(value == 1 && changed) {
        changed = false;
        return true;
    }
    else return false;
}

bool PS2key::pressed() {return (value == 0);}

//******************************

bool Cytron_PS2Shield::checkupdatetime() {
    if((millis() - tlast) < updatems) return false;
        else {
            tlast += updatems;  // set the timer for next interval
            return true;
        }
}

bool Cytron_PS2Shield::query(uint8_t key, int &value) {
    if (!checkupdatetime()) return false;
    value = readButton(key);
    return true;
}

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

bool Cytron_PS2Shield::query() {
    if (!checkupdatetime()) return false;
    for(auto& keyptr : keylist) {
    uint8_t keyval = readButton(keyptr->name());
    keyptr->changed = (keyval != keyptr->value);
    keyptr->value = keyval;
    }
    return true;
}

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

void Cytron_PS2Shield::fetch(uint8_t key, int& value)
{
    value = switchValues[key];
    return ;
}

void Cytron_PS2Shield::fetch(PS2key &key)
{
    int keyval = switchValues[key.name()];
    key.changed = keyval != key.value;
    key.value = keyval;
    return;
}

void Cytron_PS2Shield::fetch()
{
    for(auto &key : keylist) {
        fetch(key);
    }
    return;
}

void Cytron_PS2Shield::clearkeys() {
    keylist.clear();
}

void Cytron_PS2Shield::pushkey(PS2key &key) {  // add a key to the query list
      keylist.push_back(&key);
      Serial.print("Pushkey "); Serial.println(key.name());
}

PS2key& Cytron_PS2Shield::createKey(uint8_t name)
{   PS2key* keyptr = new PS2key(name);
    pushkey(*keyptr);
    return *keyptr;
}

    
/* this one needs some thought
void Cytron_PS2Shield::pushkey(int name, int &variable) {
    PS2key key(name);
    key.name = name;
    key.variable = &variable;
    key.changed = false;
    this -> pushkey(key);
}
*/

void Cytron_PS2Shield::setUpdateTime(int mS) {
    updatems = mS;
}
