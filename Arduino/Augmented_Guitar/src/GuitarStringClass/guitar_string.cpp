#include "guitar_string.h"
#include "../MIDI/midi.h"

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
      pressed_fret = j + 1;
    }
    is_last_fret_touched = is_fret_touched;
  }

  if (!any_fret_touched) {
    pressed_fret = 0;
  }

  if (pressed_fret != _last_sent_pressed_fret) {
    MIDI_lift_fret(_string_number, _last_sent_pressed_fret);
    MIDI_press_fret(_string_number, pressed_fret);
    _last_sent_pressed_fret = pressed_fret;
  }

}
