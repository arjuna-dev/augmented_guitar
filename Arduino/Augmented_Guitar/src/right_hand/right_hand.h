#ifndef RIGHT_HAND_H
#define RIGHT_HAND_H

#define hysteresis 25
#define sine_wave_falling_edge 35
#define peak_diff_threshold 30

constexpr int NUM_OF_STRINGS = 6;

extern const int string_input_pins[NUM_OF_STRINGS];
extern const char open_string_notes[NUM_OF_STRINGS];
extern const int max_amplitudes[NUM_OF_STRINGS];
extern const int min_thresholds[NUM_OF_STRINGS];
extern const int max_wave_periods[NUM_OF_STRINGS];

void setup_right_hand();
void update_peak_value(struct StringStruct& string);

#endif