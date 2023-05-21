#ifndef GUITAR_STRING_TESTABLE_H
#define GUITAR_STRING_TESTABLE_H

#include "../device_specs/device_specs.h"
#include "guitar_string.h"
#include <vector>

using namespace std;

class GuitarStringTestable : public GuitarString {

  public:
    GuitarStringTestable(
      const int string_number = 0,
      const int input_pin = 0,
      const char open_string_note = 0,
      const int max_amplitude = 0,
      const int min_threshold = 0,
      const int max_wave_period = 0,
      vector<int> mock_sine_wave_vector = {}
    ) : 
    GuitarString(
      string_number,
      input_pin,
      open_string_note,
      max_amplitude,
      min_threshold,
      max_wave_period
    ),
    _mock_sine_wave_vector(mock_sine_wave_vector),
    _mock_sine_wave_index(0)
    {};
    bool get_note_on();
    void set_note_on(bool note_on);
    void set_note_on_timestamp(unsigned long  millis);
    void set_current_amplitude(int amplitude);
    vector<int> _mock_sine_wave_vector;
  private:
    int analog_reader_right_hand(int pin) override;
    int analog_reader_left_hand(int pin) override;
    int _mock_sine_wave_index = 0;
};


#endif