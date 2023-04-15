#ifndef GUITAR_STRING_H
#define GUITAR_STRING_H

#include "../device_specs/device_specs.h"


class GuitarString {

public:
  GuitarString(const int string_number = 0, const int input_pin = 0, const char open_string_note = 0, const int max_amplitude = 0, const int min_threshold = 0, const int max_wave_period = 0, int* touch_analog_values = 0, int* touch_reference_analog_values = 0);
  void updateStringMIDIValue();
  int pressed_fret = 0;

private:
  int _string_number;
  int _input_pin;
  int _open_string_note;
  int _max_amplitude;
  int _min_threshold;
  int _max_wave_period;
  int _capacitance_threshold = 5000;
  int* _touch_analog_values;
  int* _touch_reference_analog_values;

  bool _note_on = false;
  unsigned long _note_on_timestamp = 0;
  int _current_amplitude = 0;
  int _previous_amplitude = 0;
  int _peak_value = 0;
  int _last_peak_value = 0;
  int _last_sent_pressed_fret = 0;
  int _last_sent_note_on_fret = 0;
};


#endif