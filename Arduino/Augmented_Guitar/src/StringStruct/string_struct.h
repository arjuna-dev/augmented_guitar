#ifndef STRING_STRUCT_H
#define STRING_STRUCT_H

#include "../device_specs/device_specs.h"

struct StringStruct {
  // Strumming detection
  int max_wave_period;
  bool note_on;
  unsigned long note_on_timestamp;
  int open_string_note;
  int input_pin;
  int current_amplitude;
  int previous_amplitude;
  int peak_value;
  int last_peak_value;
  int min_threshold;
  int max_amplitude;

  // Finger position detection
  int pressed_fret;
  int last_sent_pressed_fret;
  int last_sent_note_on_fret;
};

extern struct StringStruct string_structs[NUM_OF_STRINGS];

#endif