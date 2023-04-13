#include "guitar_string.h"

GuitarString::GuitarString( int input_pin, int open_string_note, int max_amplitude, int min_threshold, int max_wave_period) {
  _input_pin  = input_pin;
  _open_string_note  = open_string_note;
  _max_amplitude  = max_amplitude;
  _min_threshold  = min_threshold;
  _max_wave_period  = max_wave_period;
}