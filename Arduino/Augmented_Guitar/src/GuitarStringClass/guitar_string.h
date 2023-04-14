#ifndef GUITAR_STRING_H
#define GUITAR_STRING_H

#include "../device_specs/device_specs.h"


class GuitarString {

public:
  GuitarString(const int string_number = 0, const int input_pin = 0, const char open_string_note = 0, const int max_amplitude = 0, const int min_threshold = 0, const int max_wave_period = 0, int* touch_analog_values = 0, int* touch_reference_analog_values = 0);
  bool note_on = false;
  unsigned long note_on_timestamp = 0;
  int current_amplitude = 0;
  int previous_amplitude = 0;
  int peak_value = 0;
  int last_peak_value = 0;
  int pressed_fret = 0;
  int last_sent_pressed_fret = 0;
  int last_sent_note_on_fret = 0;
  int touch_analog_values[NUM_OF_FRETS];
  int touch_reference_analog_values[NUM_OF_FRETS];

  void updateStringMIDIValue();

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
};


#endif