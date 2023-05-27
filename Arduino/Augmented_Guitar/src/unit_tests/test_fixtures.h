#ifndef TEST_FIXTURES_H
#define TEST_FIXTURES_H

#include "AUnit.h"
#include "../GuitarStringClass/guitar_string.h"
#include "../GuitarStringClass/guitar_string_friend.h"
#include "../AnalogReader/analog_reader_interface.h"
#include "../MIDI/midi_interface.h"
#include "mock_values.h"
#include <vector>
using namespace std;

class GuitarStringFixture: public aunit::TestOnce {
  protected:
    vector<vector<int>> mock_pressed_frets_values;
    MIDIInterface* midi_methods_mock;
    vector<AnalogReaderInterface*> analog_reader_mocks;
    vector<GuitarString> guitar_string_mocks;
    GuitarStringFriend guitar_string_friend;
    vector<int> _strings_sine_wave_mocks[NUM_OF_STRINGS];
  public:
    GuitarStringFixture();
    void setup() override;
    void teardown() override;
};

#endif