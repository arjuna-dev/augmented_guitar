#ifndef MOCK_VALUES_H
#define MOCK_VALUES_H

#include <cstdlib>
#include "../device_specs/device_specs.h"
#include <vector>

using namespace std;

void selectMuxChannelMock(int mux_ch);

// GuitarStrings
extern const int string_input_pins_mock[NUM_OF_STRINGS];
extern const char open_string_notes_mock[NUM_OF_STRINGS];
extern const int max_amplitudes_mock[NUM_OF_STRINGS];
extern const int min_thresholds_mock[NUM_OF_STRINGS];
extern const int max_wave_periods_mock[NUM_OF_STRINGS];

extern CC_Types global_last_CC_mock;
extern int global_last_note_mock;
extern int global_last_velocity_mock;

extern vector<int> amplitude_mock_values_empty;
extern vector<int> amplitude_mock_values_E;
extern vector<int> amplitude_mock_values_A;
extern vector<int> amplitude_mock_values_D;
extern vector<int> amplitude_mock_values_G;
extern vector<int> amplitude_mock_values_B;
extern vector<int> amplitude_mock_values_e;
extern vector<int> strings_sine_wave_mocks[NUM_OF_STRINGS];


#endif
