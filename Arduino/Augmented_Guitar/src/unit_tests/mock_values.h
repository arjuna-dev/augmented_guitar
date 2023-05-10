#ifndef MOCK_VALUES_H
#define MOCK_VALUES_H

#include <cstdlib>
#include "../device_specs/device_specs.h"

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
int analogReadMock(int _input_pin);
extern int* ptr_mock_amplitude_values;
extern int amplitude_mock_values_E[1000];
extern int amplitude_mock_values_A[1000];
extern int amplitude_mock_values_D[1000];
extern int amplitude_mock_values_G[1000];
extern int amplitude_mock_values_B[1000];
extern int amplitude_mock_values_e[1000];
extern int* ptr_amplitude_mock_values_E;
extern int* ptr_amplitude_mock_values_A;
extern int* ptr_amplitude_mock_values_D;
extern int* ptr_amplitude_mock_values_G;
extern int* ptr_amplitude_mock_values_B;
extern int* ptr_amplitude_mock_values_e;


#endif
