#include "Arduino.h"
#include "guitar_string_testable.h"

void GuitarStringTestable::set_note_on(bool note_on){
  _note_on = note_on;
};

void GuitarStringTestable::set_note_on_timestamp(){
  _note_on_timestamp = millis();
};