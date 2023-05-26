#include "Arduino.h"
#include "analog_reader_mock.h"
#include "../device_specs/device_specs.h"


AnalogReaderMock::AnalogReaderMock(const vector<int>& mock_sine_wave_vector, vector<vector<int>>& mock_pressed_frets_arr) : 
  _mock_sine_wave_vector(mock_sine_wave_vector),
  _mock_pressed_frets_arr(mock_pressed_frets_arr),
  _mock_sine_wave_index(0),
  _mock_fret_index(0)
  {};

int AnalogReaderMock::analogReaderLeft(int pin) {
  if (_mock_fret_index >= NUM_OF_FRETS){
    _mock_fret_index = 0;
  }
  return _mock_pressed_frets_arr[_string_number][_mock_fret_index++];
}

int AnalogReaderMock::analogReaderRight(int pin) {
  return _mock_sine_wave_vector[_mock_sine_wave_index++];
}

void AnalogReaderMock::setFretValue(int string_index, int fret_index, int value) {
  _mock_pressed_frets_arr[string_index][fret_index] = value;
}
