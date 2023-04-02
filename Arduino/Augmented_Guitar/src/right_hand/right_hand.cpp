#include "Arduino.h"
#include "right_hand.h"
#include "../StringStruct/string_struct.h"

const int string_input_pins[NUM_OF_STRINGS] = {34, 35, 36, 37, 38, 39};
const char open_string_notes[NUM_OF_STRINGS] = {'E', 'A', 'D', 'G', 'B', 'e'};
const int max_amplitudes[NUM_OF_STRINGS] = {540, 510, 560, 580, 620, 600};
const int min_thresholds[NUM_OF_STRINGS] = {35, 40, 35, 40, 35, 30};
const int max_wave_periods[NUM_OF_STRINGS] = {15, 10, 8, 6, 4, 3};

void setup_right_hand() {
  for (int i = 0; i < 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }
}

void update_peak_value(struct StringStruct& string) {
  if (string.current_amplitude > string.min_threshold) {
    if (string.current_amplitude < string.previous_amplitude - sine_wave_falling_edge) {
      string.peak_value = string.previous_amplitude;
      string.peak_value = map(string.peak_value, 0, string.max_amplitude, 0, 127);
      if (string.peak_value > 127) {
        string.peak_value = 127;
      }
    } else {
      string.peak_value = 0;
    }
  } else {
    string.peak_value = 0;
  }
  string.previous_amplitude = string.current_amplitude;
}