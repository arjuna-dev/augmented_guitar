#ifndef MIDI_INTERFACE_H
#define MIDI_INTERFACE_H

enum CC_Types {
  NOTE_ON = 20,
  NOTE_OFF = 21,
  PRESS_FRET = 22,
  LIFT_FRET = 23,
  UNDEFINED = -1
};

class MIDIInterface {
  public:
    virtual ~MIDIInterface() = default;
    virtual void MIDI_press_fret(int string, int fret);
    virtual void MIDI_lift_fret(int string, int fret);
    virtual void MIDI_note_on(int string, int velocity, int fret);
    virtual void MIDI_note_off(int string, int fret);
};

#endif
