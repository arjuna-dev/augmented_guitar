#include "Arduino.h"
#include "guitar_string.h"
#include "../MIDI/midi.h"
#include "../debug/debug.h"

#define sine_wave_falling_edge 35
#define peak_diff_threshold 30

GuitarString::GuitarString(const int string_number, const int input_pin, const char open_string_note, const int max_amplitude, const int min_threshold, const int max_wave_period, int* touch_analog_values, int* touch_reference_analog_values) {
  _string_number  = string_number;
  _open_string_note  = open_string_note;
  _max_amplitude  = max_amplitude;
  _touch_analog_values  = touch_analog_values;
  _touch_reference_analog_values  = touch_reference_analog_values;
  _max_wave_period  = max_wave_period;
  _input_pin  = input_pin;
  _min_threshold  = min_threshold;
}


void GuitarString::updateStringMIDIValue() {
  bool is_last_fret_touched = true;
  bool is_fret_touched = false;
  bool any_fret_touched = false;
  
  for (int j = 0; j < 4; j++) {

    if (_touch_analog_values[value_positions[_string_number][j]] - _touch_reference_analog_values[value_positions[_string_number][j]] > _capacitance_threshold) {
      is_fret_touched = true;
      any_fret_touched = true;
    } else {
      is_fret_touched = false;
    }
    if (is_last_fret_touched && is_fret_touched && !_note_on) {
      _pressed_fret = j + 1;
    }
    is_last_fret_touched = is_fret_touched;
  }

  if (!any_fret_touched) {
    _pressed_fret = 0;
  }

  if (_pressed_fret != _last_sent_pressed_fret) {
    MIDI_lift_fret(_string_number, _last_sent_pressed_fret);
    MIDI_press_fret(_string_number, _pressed_fret);
    _last_sent_pressed_fret = _pressed_fret;
  }

}

void GuitarString::update_current_amplitude() {
  _current_amplitude = analogRead(_input_pin);
}

void GuitarString::detect_note_off() {
  if (_note_on) {
    // Threshold crossed, reset note_on_timestamp
    if (_current_amplitude > _min_threshold) {
      _note_on_timestamp = millis();
    }

    if (_note_on_timestamp + _max_wave_period < millis()) {
      MIDI_note_off(_string_number, _last_sent_note_on_fret);
      _note_on = false;
    }
  }
}

void GuitarString::update_peak_value() {
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
}

void GuitarString::detect_note_on() {
  if (_peak_value > _last_peak_value + peak_diff_threshold && !_note_on) {
    _note_on = true;
    _note_on_timestamp = millis();
    MIDI_note_on(_string_number, _peak_value, _pressed_fret);
    _last_sent_note_on_fret = _pressed_fret;
  }
}

void GuitarString::update_last_peak_value() {
  if (_peak_value) {
      _last_peak_value = _peak_value;
  }
}

void GuitarString::update_previous_amplitude() {
  _previous_amplitude = _current_amplitude;
}


int GuitarString::get_MIDI_value() {
  return MIDI_open_string_notes[_pressed_fret];
}

void GuitarString::detect_note_on_off() {
  update_current_amplitude();
  detect_note_off();
  update_peak_value();
  detect_note_on();
  update_last_peak_value();
  update_previous_amplitude();
}

int GuitarString::get_current_amplitude(){
  return _current_amplitude;
}

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// _-_-_-_-_-_-_-_-_-_-_- Debug _-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#define NUM_OF_ARR_VALUES 1000
#define hysteresis 25
bool sine_wave_started = false;
int record_sine_wave_counter = 0;
int sine_wave_array[NUM_OF_ARR_VALUES];

void GuitarString::printSineWaveValues(int iteration, int number_of_iterations) {
  if (_string_number == iteration) {

    if (sine_wave_started == false && _current_amplitude > _min_threshold && _current_amplitude > _previous_amplitude + hysteresis) {
      Serial.println("in");
      sine_wave_started = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (sine_wave_started && record_sine_wave_counter < number_of_iterations) {
      sine_wave_array[record_sine_wave_counter] = _current_amplitude;
      record_sine_wave_counter++;
    }
    if (record_sine_wave_counter == number_of_iterations - 1) {
      for (int j = 0; j < number_of_iterations - 1; j++) {
        Serial.println(sine_wave_array[j]);
      }
    }
  }
}