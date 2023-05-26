#include "Arduino.h"
#include "guitar_string_mock.h"


GuitarStringMock::GuitarStringMock(      
  MIDIInterface* midi_methods,
  const int string_number,
  const int input_pin,
  const char open_string_note,
  const int max_amplitude,
  const int min_threshold,
  const int max_wave_period,
  const vector<int> mock_sine_wave_vector,
  vector<vector<int>>& mock_pressed_frets_arr) :
  GuitarString(
    midi_methods,
    string_number,
    input_pin,
    open_string_note,
    max_amplitude,
    min_threshold,
    max_wave_period
  ),
  _mock_sine_wave_vector(mock_sine_wave_vector),
  _mock_sine_wave_index(0),
  _mock_pressed_frets_arr(mock_pressed_frets_arr),
  _mock_fret_index(0)
  {};

void GuitarStringMock::set_note_on(bool note_on){
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

void GuitarStringMock::set_pressed_fret(int fret, int value) {
    _mock_pressed_frets_arr[_string_number][fret] = value;
}