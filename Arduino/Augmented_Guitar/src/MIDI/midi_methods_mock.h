#ifndef MIDI_METHODS_MOCK_H
#define MIDI_METHODS_MOCK_H

#include "../device_specs/device_specs.h"
#include "midi_interface.h"

class MIDIMethodsMock : public MIDIInterface {
  public:
    MIDIMethodsMock();
    void MIDI_press_fret(int string, int fret) override;
    void MIDI_lift_fret(int string, int fret) override;
    void MIDI_note_on(int string, int velocity, int fret) override;
    void MIDI_note_off(int string, int fret) override;
    int get_last_sent_CC_value();
    int get_last_sent_MIDI_note_value();
    int get_last_sent_MIDI_velocity_value();
  private:
    int _sent_CC_value;
    int _sent_MIDI_note_value;
    int _sent_MIDI_velocity_value;
};

#endif
