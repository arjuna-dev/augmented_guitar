#include "Arduino.h"
#include "midi_methods_mock.h"
#include "../unit_tests/mock_values.h"

MIDIMethodsMock::MIDIMethodsMock() {}

void MIDIMethodsMock::MIDI_press_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  global_last_CC_mock = PRESS_FRET;
  global_last_note_mock = note;
  global_last_velocity_mock = 0;
}

void MIDIMethodsMock::MIDI_lift_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  global_last_CC_mock = LIFT_FRET;
  global_last_note_mock = note;
  global_last_velocity_mock = 0;
}

void MIDIMethodsMock::MIDI_note_on(int string, int velocity, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  global_last_CC_mock = NOTE_ON;
  global_last_note_mock = note;
  global_last_velocity_mock = velocity;
}

void MIDIMethodsMock::MIDI_note_off(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  global_last_CC_mock = NOTE_OFF;
  global_last_note_mock = note;
  global_last_velocity_mock = 0;
}

int MIDIMethodsMock::get_last_sent_CC_value(){
  return _sent_CC_value;
}

int MIDIMethodsMock::get_last_sent_MIDI_note_value(){
  return _sent_MIDI_note_value;
}

int MIDIMethodsMock::get_last_sent_MIDI_velocity_value(){
  return _sent_MIDI_velocity_value;
}