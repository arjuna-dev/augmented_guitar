#include "Arduino.h"
#include "midi_methods.h"

MIDIMethods::MIDIMethods() {}

void MIDIMethods::MIDI_press_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  usbMIDI.sendControlChange(PRESS_FRET, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
}

void MIDIMethods::MIDI_lift_fret(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  usbMIDI.sendControlChange(LIFT_FRET, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
}

void MIDIMethods::MIDI_note_on(int string, int velocity, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  usbMIDI.sendControlChange(NOTE_ON, fret, 1);
  usbMIDI.sendNoteOn(note, velocity, 1);
}

void MIDIMethods::MIDI_note_off(int string, int fret) {
  int note = MIDI_open_string_notes[string] + fret;
  usbMIDI.sendControlChange(NOTE_OFF, fret, 1);
  usbMIDI.sendNoteOff(note, 0, 1);
}
