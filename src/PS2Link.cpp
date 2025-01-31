/**************************************
*   Additions to Cytron_PS2Shield     *
*   to incorporate PS2Link functions  *
**************************************/

#include "Cytron_PS2_JD.h"

bool Cytron_PS2Shield::checkupdatetime() {
    if((millis() - tlast) < updatems) return false;
        else {
            tlast += updatems;  // set the timer for next interval
            return true;
        }
}

bool Cytron_PS2Shield::query(uint8_t key, int &value) {
    if (!checkupdatetime()) return false;
    value = this->readButton(key);
    return true;
}
