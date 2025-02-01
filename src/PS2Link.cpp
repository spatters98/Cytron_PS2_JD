/**************************************
*   Additions to Cytron_PS2Shield     *
*   to incorporate PS2Link functions  *
**************************************/

#include "Cytron_PS2_JD.h"

PS2key::PS2key(uint8_t keyname) {kname = keyname;}
uint8_t PS2key::name() { return kname;}

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

void Cytron_PS2Shield::pushkey(PS2key &key) {  // add a key to the query list
      keylist.push_back(&key);
      Serial.print("Pushkey "); Serial.println(key.name());
}
    
void Cytron_PS2Shield::clearkeys() {
    keylist.clear();
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
