#ifndef MOCK_VALUES_H
#define MOCK_VALUES_H

#include <cstdlib>
#include "../device_specs/device_specs.h"
#include <vector>

using namespace std;

extern int mock_mock_array[1];

// TeensyTouch
extern int mux_pins_mock[NUM_OF_MUX_PINS];
extern int* ptr_mux_pins_mock;
extern int mux_ch_mock;
extern int* ptr_mux_ch_mock ;
extern int analog_values_mock[NUM_OF_NOTES];
extern int* ptr_analog_values_mock;
extern int reference_analog_values_mock[NUM_OF_NOTES];
extern int* ptr_reference_analog_values_mock;
void selectMuxChannelMock(int mux_ch);
extern int* ptr_mock_TSI_values;

// GuitarStrings
extern const int string_input_pins_mock[NUM_OF_STRINGS];
extern const char open_string_notes_mock[NUM_OF_STRINGS];
extern const int max_amplitudes_mock[NUM_OF_STRINGS];
extern const int min_thresholds_mock[NUM_OF_STRINGS];
extern const int max_wave_periods_mock[NUM_OF_STRINGS];

extern vector<int> amplitude_mock_values_empty;
extern vector<int> amplitude_mock_values_E;
extern vector<int> amplitude_mock_values_A;
extern vector<int> amplitude_mock_values_D;
extern vector<int> amplitude_mock_values_G;
extern vector<int> amplitude_mock_values_B;
extern vector<int> amplitude_mock_values_e;
extern vector<int> strings_sine_wave_mocks[NUM_OF_STRINGS];


#endif
