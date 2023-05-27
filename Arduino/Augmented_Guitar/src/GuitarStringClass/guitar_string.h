#ifndef GUITAR_STRING_H
#define GUITAR_STRING_H

#include "../device_specs/device_specs.h"
#include "../AnalogReader/analog_reader_interface.h"
#include "../MIDI/midi_interface.h"


class GuitarString {

public:
  GuitarString(
    AnalogReaderInterface* analog_reader,
    MIDIInterface* midi_methods,
    const int string_number,
    const int input_pin,
    const char open_string_note,
    const int max_amplitude,
    const int min_threshold,
    const int max_wave_period);

    void detect_note_on(bool debug_sine_wave=false, int string_number=0 , int number_of_values=1000);
    void detect_note_off();
    void update_string_MIDI_value();
    void update_prev_and_current_amplitudes();
    int get_MIDI_value();  
    int getAnalogValues();

  friend class GuitarStringFriend;

protected:
  void detect_peak_value();
  void update_last_peak_value();
  void printSineWaveValues(int iteration, int number_of_iterations);
  void detect_peak_value_soft();

  AnalogReaderInterface* _analog_reader;
  MIDIInterface* _midi_methods;
  int _string_number;
  int _input_pin;
  int _open_string_note;
  int _max_amplitude;
  int _min_threshold;
  int _max_wave_period;

  bool _note_on;
  int _pressed_fret;
  unsigned long _note_on_timestamp;
  int _current_amplitude;
  int _previous_amplitude;
  int _peak_value;
  int _last_peak_value;
  int _last_sent_pressed_fret;
  int _last_sent_note_on_fret;
  int _accumulated_decrements;
  int _trough_count;
};


#endif