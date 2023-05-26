#include "Arduino.h"
#include "midi_methods_mock.h"

MIDIMethodsMock::MIDIMethodsMock() {}

void MIDIMethodsMock::MIDI_press_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  _sent_CC_value = PRESS_FRET;
  _sent_MIDI_note_value = note;
}

void MIDIMethodsMock::MIDI_lift_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  _sent_CC_value = LIFT_FRET;
  _sent_MIDI_note_value = note;
}

void MIDIMethodsMock::MIDI_note_on(int string, int velocity, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  _sent_CC_value = NOTE_ON;
  _sent_MIDI_note_value = note;
  _sent_MIDI_velocity_value = velocity;
}

void MIDIMethodsMock::MIDI_note_off(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  _sent_CC_value = NOTE_OFF;
  _sent_MIDI_note_value = note;
  _sent_MIDI_velocity_value = 0;
}

int MIDIMethodsMock::get_last_sent_CC_value(){
  return _sent_CC_value;
};

int MIDIMethodsMock::get_last_sent_MIDI_note_value(){
  return _sent_MIDI_note_value;
};

int MIDIMethodsMock::get_last_sent_MIDI_velocity_value(){
  return _sent_MIDI_velocity_value;
}