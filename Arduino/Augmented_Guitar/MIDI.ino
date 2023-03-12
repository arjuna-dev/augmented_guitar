void MIDI_press_fret(int note, int fret) {
  usbMIDI.sendControlChange(22, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
}

void MIDI_lift_fret(int note, int fret) {
  usbMIDI.sendControlChange(23, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
}

void MIDI_note_on(int note, int velocity, int fret) {
  usbMIDI.sendControlChange(20, fret, 1);
  usbMIDI.sendNoteOn(note, velocity, 1);
}

void MIDI_note_off(int note, int fret) {
  usbMIDI.sendControlChange(21, fret, 1);
  usbMIDI.sendNoteOff(note, 0, 1);
}
