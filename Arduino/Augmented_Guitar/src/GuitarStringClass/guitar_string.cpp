#include "Arduino.h"
#include "guitar_string.h"
#include "../MIDI/midi_interface.h"
#include "../AnalogReader/analog_reader_interface.h"
#include "../debug/debug.h"
#include "../device_specs/device_specs.h"
#include "../mux/mux.h"
#include "../Settings/settings.h"
#include <algorithm>


GuitarString::GuitarString(
  AnalogReaderInterface* analog_reader,
  MIDIInterface* midi_methods,
  const int string_number,
  const int input_pin,
  const char open_string_note,
  const int max_amplitude,
  const int min_threshold,
  const int max_wave_period) :
  _analog_reader(analog_reader),
  _midi_methods(midi_methods),
  _string_number(string_number),
  _input_pin(input_pin),
  _open_string_note(open_string_note),
  _max_amplitude(max_amplitude),
  _min_threshold(min_threshold),
  _max_wave_period(max_wave_period),

  _note_on(false),
  _pressed_fret(0),
  _note_on_timestamp(0),
  _current_amplitude(0),
  _previous_amplitude(0),
  _peak_value(0),
  _last_peak_value(0),
  _last_peak_value_sent(0),
  _last_sent_pressed_fret(0),
  _last_sent_note_on_fret(0),
  _accumulated_decrements(0),
  _trough_count(0)
  {}

vector<bool> GuitarString::static_strings_pressed_states(NUM_OF_STRINGS, false);
vector<bool> GuitarString::static_strings_notes_on_states(NUM_OF_STRINGS, false);

void GuitarString::update_string_MIDI_value() {
  bool is_fret_touched = false;
  bool any_fret_touched = false;

  for (int j = 0; j < 4; j++) {
    selectMuxChannel(fret_position_to_mux[_string_number][j][1]);
    int fret_value = _analog_reader->analog_reader_left(fret_position_to_mux[_string_number][j][0]);
    if (fret_value > fret_touched_threshold) {
      is_fret_touched = true;
      any_fret_touched = true;
      static_strings_pressed_states[_string_number] = true;
    } else {
      is_fret_touched = false;
    }
    if (is_fret_touched && !_note_on) {
      _pressed_fret = j + 1;
    }
  }
  if (!any_fret_touched) {
    _pressed_fret = 0;
    static_strings_pressed_states[_string_number] = false;
  }
  
  if (_pressed_fret != _last_sent_pressed_fret) {
    _midi_methods->MIDI_lift_fret(_string_number, _last_sent_pressed_fret);
    _midi_methods->MIDI_press_fret(_string_number, _pressed_fret);
    _last_sent_pressed_fret = _pressed_fret;
  }

}

void GuitarString::detect_note_off() {
  if (_note_on) {

    // if (_pressed_fret != _last_sent_note_on_fret) {
    //   send_note_off();
    // }

    if (settings.sustain_mode == SUSTAIN_MODE_SHORT) {
      if (_note_on_timestamp + 1000 < millis()) {
        send_note_off();
      }
    } else if (settings.sustain_mode == SUSTAIN_MODE_MEDIUM) {
      if (_note_on_timestamp + 2500 < millis()) {
        send_note_off();
      }
    } else if (settings.sustain_mode == SUSTAIN_MODE_LONG) {
      if (_note_on_timestamp + 5000 < millis()) {
        send_note_off();
      }
    } else if (settings.sustain_mode == SUSTAIN_MODE_WAVEFORM) {
        // Threshold crossed, reset note_on_timestamp
        if (_current_amplitude > _min_threshold) {
          _note_on_timestamp = millis();
        }
        // If an above-threshold value didn't occur in a wave period time, send note_off
        if (_note_on_timestamp + _max_wave_period < millis()) {
          send_note_off();
        }
    } else if (settings.sustain_mode == SUSTAIN_MODE_INFINITE && settings.playing_mode == PLAYING_MODE_CHORDS_ONLY) {
      /*
      This might be redundant because of the first if statement:
      if (_pressed_fret != _last_sent_note_on_fret) {
        send_note_off();
      }
       */
      if (all_of(static_strings_pressed_states.begin(), static_strings_pressed_states.end(), [](int i){ return i == 0; })) {
        send_note_off();
      }
    } else if (settings.sustain_mode == SUSTAIN_MODE_INFINITE && settings.playing_mode == PLAYING_MODE_NOTES_ONLY) {
      if (_pressed_fret == 0) {
        if (_note_on_timestamp + 5000 < millis()) {
          send_note_off();
        }
      }
      
    }

  }
}

void GuitarString::send_note_off() {
  _midi_methods->MIDI_note_off(_string_number, _last_sent_note_on_fret);
  _note_on = false;
  static_strings_notes_on_states[_string_number] = false;
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
  _current_amplitude = _analog_reader->analog_reader_right(_input_pin);
}


void GuitarString::send_note_on() {
  _midi_methods->MIDI_note_on(_string_number, _peak_value, _pressed_fret);
  _note_on = true;
  _note_on_timestamp = millis();
  _last_sent_note_on_fret = _pressed_fret;
  _last_peak_value_sent = _peak_value;
  static_strings_notes_on_states[_string_number] = true;
}

void GuitarString::detect_note_on() {

    // Detect note on for hammer-ons/pull-ups
  if (settings.hammer_ons_pull_ups_enabled && _last_sent_note_on_fret != _pressed_fret && _pressed_fret != 0) {
    _midi_methods->MIDI_note_off(_string_number, _last_sent_note_on_fret);
    send_note_on();
  }
  
  detect_peak_value();
  // Next line to lock chord  or note (no strumming [&& !_note_on]):
  if (_peak_value > _last_peak_value + peak_diff_threshold && !_note_on) {

    int pressed_frets_count = std::count_if(static_strings_pressed_states.begin(), static_strings_pressed_states.end(), [](int i){ return i > 0; });
    int notes_on_count = std::count_if(static_strings_notes_on_states.begin(), static_strings_notes_on_states.end(), [](int i){ return i > 0; });

    if (settings.playing_mode == PLAYING_MODE_CHORDS_ONLY) {
      if (pressed_frets_count > 1) {
        send_note_on();
      }
    } else if (settings.playing_mode == PLAYING_MODE_NOTES_ONLY) {
      if (pressed_frets_count <= 1 && notes_on_count == 0) {
        send_note_on();
      }
    } else if (settings.playing_mode == PLAYING_MODE_CHORD_NOTE_AUTO) {
      if (pressed_frets_count > 1) {
        send_note_on();
      } else if (pressed_frets_count <= 1 && notes_on_count == 0) {
        send_note_on();
      }
    }
  }
  update_last_peak_value();

}

int GuitarString::get_current_amplitude() {
  return _current_amplitude;
}