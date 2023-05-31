#include "Arduino.h"
#include "analog_reader_mock.h"
#include "../device_specs/device_specs.h"


AnalogReaderMock::AnalogReaderMock(vector<int>& mock_sine_wave_vector, vector<vector<int>>& mock_pressed_frets_arr, int string_number) : 
  _mock_sine_wave_vector(mock_sine_wave_vector),
  _mock_sine_wave_index(0),
  _mock_pressed_frets_arr(mock_pressed_frets_arr),
  _mock_fret_index(0),
  _string_number(string_number)
  {};

int AnalogReaderMock::analog_reader_left(int pin) {
  if (_mock_fret_index >= NUM_OF_FRETS){
    _mock_fret_index = 0;
  }
  return _mock_pressed_frets_arr[_string_number][_mock_fret_index++];
}

int AnalogReaderMock::analog_reader_right(int pin) {
  return _mock_sine_wave_vector[_mock_sine_wave_index++];
}
