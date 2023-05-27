#include <iostream>
#include "guitar_string_friend.h"


bool GuitarStringFriend::get_note_on(GuitarString& obj) {
    return obj._note_on;
}

void GuitarStringFriend::set_note_on(GuitarString& obj, bool value) {
    obj._note_on = value;
}

void GuitarStringFriend::set_note_on_timestamp(GuitarString& obj, unsigned long value) {
    obj._note_on_timestamp = value;
}

void GuitarStringFriend::set_current_amplitude(GuitarString& obj, int value) {
    obj._current_amplitude = value;
}
