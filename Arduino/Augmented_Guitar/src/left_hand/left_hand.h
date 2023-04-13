#ifndef LEFT_HAND_H
#define LEFT_HAND_H

#include "../device_specs/device_specs.h"

#define mux_1_pin 30
#define mux_2_pin 23

constexpr int NUM_OF_NOTES = 32;
constexpr int NUM_OF_MUX_PINS = 2;

extern const int capacitance_threshold;
extern int mux_pins[NUM_OF_MUX_PINS];
extern int* ptr_mux_pins;
extern int touch_analog_values[NUM_OF_NOTES];
extern int* ptr_touch_analog_values;
extern int mux_ch;
extern int* ptr_mux_ch;
extern int touch_reference_analog_values[NUM_OF_NOTES];
extern int* ptr_touch_reference_analog_values;

// This array has maps the position of the array/fret with the order in which the frets are scanned.
// E.g. The first fret of the first string will be read on the 31st iteration (denoted by number 30)
extern const int value_positions[NUM_OF_STRINGS][NUM_OF_FRETS];

void setupLeftHand();
void updateStringMIDIValue(struct StringStruct& string, int string_number);

#endif