#ifndef MIDI_H
#define MIDI_H

constexpr int NUMB_OF_STRINGS = 6;

extern const int MIDI_open_string_notes[NUMB_OF_STRINGS];
void MIDI_press_fret(int string, int fret);
void MIDI_lift_fret(int string, int fret);
void MIDI_note_on(int string, int velocity, int fret);
void MIDI_note_off(int string, int fret);

#endif