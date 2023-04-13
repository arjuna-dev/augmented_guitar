#ifndef MIDI_H
#define MIDI_H

#include "../device_specs/device_specs.h"

extern const int MIDI_open_string_notes[NUM_OF_STRINGS];
void MIDI_press_fret(int string, int fret);
void MIDI_lift_fret(int string, int fret);
void MIDI_note_on(int string, int velocity, int fret);
void MIDI_note_off(int string, int fret);

#endif