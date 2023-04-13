#ifndef GUITAR_STRING_H
#define GUITAR_STRING_H

class GuitarString {

public:
  GuitarString(int input_pin = 0, int open_string_note = 0, int max_amplitude = 0, int min_threshold = 0, int max_wave_period = 0);
  bool note_on;
  unsigned long note_on_timestamp;
  int current_amplitude;
  int previous_amplitude;
  int peak_value;
  int last_peak_value;
  int pressed_fret;
  int last_sent_pressed_fret;
  int last_sent_note_on_fret;

private:
  int _input_pin;
  int _open_string_note;
  int _max_amplitude;
  int _min_threshold;
  int _max_wave_period;
};


#endif