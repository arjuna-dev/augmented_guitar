#ifndef MIDI_METHODS_H
#define MIDI_METHODS_H

#include "../device_specs/device_specs.h"
#include "midi_interface.h"

class MIDIMethods : public MIDIInterface {
public:
  MIDIMethods();
  void MIDI_press_fret(int string, int fret) override;
  void MIDI_lift_fret(int string, int fret) override;
  void MIDI_note_on(int string, int velocity, int fret) override;
  void MIDI_note_off(int string, int fret) override;
};

#endif
