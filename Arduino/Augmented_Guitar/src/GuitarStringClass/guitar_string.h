#ifndef GUITAR_STRING_H
#define GUITAR_STRING_H

#include "../device_specs/device_specs.h"


class GuitarString {

public:
  GuitarString(const int string_number = 0, const int input_pin = 0, const char open_string_note = 0, const int max_amplitude = 0, const int min_threshold = 0, const int max_wave_period = 0, int* touch_analog_values = 0, int* touch_reference_analog_values = 0);
  void detect_note_on_off(int (*analog_read_func)(uint8_t), bool debug_sine_wave=false, int iteration=0 , int number_of_values=1000);
  void updateStringMIDIValue();

  int get_MIDI_value();
  
  void printSineWaveValues(int iteration, int number_of_iterations);
  bool _note_on = false;

private:
  void detect_note_off();
  void update_peak_value();
  void detect_note_on();
  void update_last_peak_value();

  int _pressed_fret = 0;
  int _string_number;
  int _input_pin;
  int _open_string_note;
  int _max_amplitude;
  int _min_threshold;
  int _max_wave_period;
  int _capacitance_threshold = 5000;
  int* _touch_analog_values;
  int* _touch_reference_analog_values;
  unsigned long _note_on_timestamp = 0;
  int _current_amplitude = 0;
  int _previous_amplitude = 0;
  int _peak_value = 0;
  int _last_peak_value = 0;
  int _last_sent_pressed_fret = 0;
  int _last_sent_note_on_fret = 0;
};


#endif