#ifndef ANALOG_READER_MOCK_H
#define ANALOG_READER_MOCK_H

#include "analog_reader_interface.h"
#include <vector>

using namespace std;

class AnalogReaderMock : public AnalogReaderInterface
{
  public:
    AnalogReaderMock(const vector<int>& mock_sine_wave_vector, vector<vector<int>>& mock_pressed_frets_arr);
    void setFretValue(int string_index, int fret_index, int value);
  protected:
    int analogReaderLeft(int pin) override;
    int analogReaderRight(int pin) override;
  private:
    vector<int> _mock_sine_wave_vector;
    int _mock_sine_wave_index;
    vector<vector<int>>& _mock_pressed_frets_arr;
    int _mock_fret_index;
};

#endif
