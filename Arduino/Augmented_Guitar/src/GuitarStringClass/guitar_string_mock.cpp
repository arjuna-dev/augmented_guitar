#include "Arduino.h"
#include "guitar_string_testable.h"

void GuitarStringTestable::set_note_on(bool note_on){
  _note_on = note_on;
};

void GuitarStringTestable::set_note_on_timestamp(unsigned long millis){
  _note_on_timestamp = millis;
};

void GuitarStringTestable::set_current_amplitude(int amplitude){
  _current_amplitude = amplitude;
};

bool GuitarStringTestable::get_note_on(){
 return _note_on;
};

int GuitarStringTestable::analog_reader_right_hand(int pin){
  return _mock_sine_wave_vector[_mock_sine_wave_index++];
}

int GuitarStringTestable::analog_reader_left_hand(int mux_pin){
  if (_mock_fret_index >= NUM_OF_FRETS){
    _mock_fret_index = 0;
  }
  return _mock_pressed_frets_arr[_string_number][_mock_fret_index++];
}