#ifndef GUITAR_STRING_TESTABLE_H
#define GUITAR_STRING_TESTABLE_H

#include "../device_specs/device_specs.h"
#include "guitar_string.h"

class GuitarStringTestable : public GuitarString {

public:
  GuitarStringTestable(
    const int string_number = 0,
    const int input_pin = 0,
    const char open_string_note = 0,
    const int max_amplitude = 0,
    const int min_threshold = 0,
    const int max_wave_period = 0,
    int* touch_analog_values = 0,
    int* touch_reference_analog_values = 0
  ) : 
  GuitarString(
    string_number,
    input_pin,
    open_string_note,
    max_amplitude,
    min_threshold,
    max_wave_period,
    touch_analog_values,
    touch_reference_analog_values
  ){};

  void set_note_on(bool note_on);
  void set_note_on_timestamp(unsigned long  millis);
  void set_current_amplitude(int amplitude);

};


#endif