#include "Arduino.h"
#include "guitar_string.h"
#include "../MIDI/midi_interface.h"
#include "../debug/debug.h"
#include "../device_specs/device_specs.h"
#include "../mux/mux.h"

#define sine_wave_falling_edge 35
#define peak_diff_threshold 30
#define _fret_touched_threshold 950

GuitarString::GuitarString(
  MIDIInterface* midi_methods,
  const int string_number,
  const int input_pin,
  const char open_string_note,
  const int max_amplitude,
  const int min_threshold,
  const int max_wave_period) :
  _midi_methods(midi_methods),
  _string_number(string_number),
  _input_pin(input_pin),
  _open_string_note(open_string_note),
  _max_amplitude(max_amplitude),
  _min_threshold(min_threshold),
  _max_wave_period(max_wave_period){}

int GuitarString::analog_reader_right_hand(int pin){
  return analogRead(pin);
}

int GuitarString::analog_reader_left_hand(int pin){
  return analogRead(pin);
}

void GuitarString::updateStringMIDIValue() {
  bool is_fret_touched = false;
  bool any_fret_touched = false;

  for (int j = 0; j < 4; j++) {
    selectMuxChannel(fret_position_to_mux[_string_number][j][1]);
    int fret_value = analog_reader_left_hand(fret_position_to_mux[_string_number][j][0]);
    if (fret_value > _fret_touched_threshold) {
      is_fret_touched = true;
      any_fret_touched = true;
    } else {
      is_fret_touched = false;
    }
    if (is_fret_touched && !_note_on) {
      _pressed_fret = j + 1;
    }
  }
  if (!any_fret_touched) {
    _pressed_fret = 0;
  }

  if (_pressed_fret != _last_sent_pressed_fret) {
    _midi_methods->MIDI_lift_fret(_string_number, _last_sent_pressed_fret);
    _midi_methods->MIDI_press_fret(_string_number, _pressed_fret);
    _last_sent_pressed_fret = _pressed_fret;
  }

}

void GuitarString::detect_note_off() {
  if (_note_on) {
    // Threshold crossed, reset note_on_timestamp
    if (_current_amplitude > _min_threshold) {
      _note_on_timestamp = millis();
    }

    // If an above-threshold value didn't occur in a wave period time, send note_off
    if (_note_on_timestamp + _max_wave_period < millis()) {
      _midi_methods->MIDI_note_off(_string_number, _last_sent_note_on_fret);
      _note_on = false;
    }
  }
}

void GuitarString::detect_peak_value_soft() {
  if (_current_amplitude <= _previous_amplitude) {
    _accumulated_decrements += _current_amplitude;
  } else {
    _accumulated_decrements = 0;
  }

  if ((_current_amplitude == 0 && _previous_amplitude == 0)||(_current_amplitude == 1 && _previous_amplitude == 1)){ 
    _trough_count++;
  } else {
    _trough_count = 0;
  }
  if (_trough_count >= 5) {
    _peak_value = _accumulated_decrements;
    _peak_value = map(_peak_value, 0, _max_amplitude, 0, 127);
    if (_peak_value > 127) {
      _peak_value = 127;
    }
  }
}

void GuitarString::detect_peak_value() {
  if (_current_amplitude > _min_threshold) {
    if (_current_amplitude < _previous_amplitude - sine_wave_falling_edge) {
      _peak_value = _previous_amplitude;
      _peak_value = map(_peak_value, 0, _max_amplitude, 0, 127);
      if (_peak_value > 127) {
        _peak_value = 127;
      }
    } else {
      _peak_value = 0;
    }
  } else {
    _peak_value = 0;
  }
  detect_peak_value_soft();
}

void GuitarString::update_last_peak_value() {
  if (_peak_value) {
      _last_peak_value = _peak_value;
  }
}

int GuitarString::get_MIDI_value() {
  return MIDI_open_string_notes[_string_number]+_pressed_fret;
}

void GuitarString::update_prev_and_current_amplitudes() {
  _previous_amplitude = _current_amplitude;
  _current_amplitude = analog_reader_right_hand(_input_pin);
}


void GuitarString::detect_note_on(bool debug_sine_wave, int string_number, int number_of_values) {

  if (debug_sine_wave == true) {
    printSineWaveValues(string_number, number_of_values);
  }
  
  detect_peak_value();
  if (_peak_value > _last_peak_value + peak_diff_threshold && !_note_on) {
    _note_on = true;
    _note_on_timestamp = millis();
    _midi_methods->MIDI_note_on(_string_number, _peak_value, _pressed_fret);
    _last_sent_note_on_fret = _pressed_fret;
  }
  update_last_peak_value();

}

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// _-_-_-_-_-_-_-_-_-_-_- Debug _-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#define NUM_OF_ARR_VALUES 1000
#define hysteresis 25
bool sine_wave_started = false;
int record_sine_wave_counter = 0;
int sine_wave_array[NUM_OF_ARR_VALUES];

void GuitarString::printSineWaveValues(int string_number, int number_of_values) {
  if (_string_number == string_number) {

    if (sine_wave_started == false && _current_amplitude > _min_threshold && _current_amplitude > _previous_amplitude + hysteresis) {
      sine_wave_started = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (sine_wave_started && record_sine_wave_counter < number_of_values) {
      sine_wave_array[record_sine_wave_counter] = _current_amplitude;
      record_sine_wave_counter++;
    }
    if (record_sine_wave_counter == number_of_values - 1) {
      for (int j = 0; j < number_of_values - 1; j++) {
        Serial.println(sine_wave_array[j]);
      }
    }
  }
}

int GuitarString::getAnalogValues() {
  return _current_amplitude;
}