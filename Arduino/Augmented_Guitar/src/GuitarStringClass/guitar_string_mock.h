#ifndef GUITAR_STRING_TESTABLE_H
#define GUITAR_STRING_TESTABLE_H

#include "../device_specs/device_specs.h"
#include "guitar_string.h"
#include <vector>

using namespace std;

class GuitarStringTestable : public GuitarString {

  public:
    GuitarStringMock(
      MIDIInterface* midi_methods,
      const int string_number,
      const int input_pin,
      const char open_string_note,
      const int max_amplitude,
      const int min_threshold,
      const int max_wave_period,
      const vector<int> mock_sine_wave_vector,
      vector<vector<int>>& mock_pressed_frets_arr
    );
    bool get_note_on();
    void set_note_on(bool note_on);
    void set_note_on_timestamp(unsigned long  millis);
    void set_current_amplitude(int amplitude);
    void set_pressed_fret(int fret, int value);
    vector<int> _mock_sine_wave_vector;
    vector<vector<int>> _mock_pressed_frets_arr;
  private:
    int analog_reader_right_hand(int pin) override;
    int analog_reader_left_hand(int pin) override;
    int _mock_sine_wave_index = 0;
    int _mock_fret_index = 0;
};


#endif