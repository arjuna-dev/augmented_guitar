#ifndef DEVICE_SPECS_H
#define DEVICE_SPECS_H

constexpr int mux_sig_pin_2 = 31;
constexpr int mux_sig_pin_1 = 23;

constexpr int NUM_OF_STRINGS = 6;
constexpr int NUM_OF_FRETS = 4;
constexpr int NUM_OF_NOTES = 32;
constexpr int NUM_OF_MUX_PINS = 2;

constexpr int fret_position_to_mux[NUM_OF_STRINGS][NUM_OF_FRETS][2] = {
  {{mux_sig_pin_2, 15}, {mux_sig_pin_2, 9}, {mux_sig_pin_2, 3}, {mux_sig_pin_1, 2}},
  {{mux_sig_pin_2, 14}, {mux_sig_pin_2, 8}, {mux_sig_pin_2, 2}, {mux_sig_pin_1, 3}},
  {{mux_sig_pin_2, 13}, {mux_sig_pin_2, 7}, {mux_sig_pin_2, 1}, {mux_sig_pin_1, 4}},
  {{mux_sig_pin_2, 12}, {mux_sig_pin_2, 6}, {mux_sig_pin_2, 0}, {mux_sig_pin_1, 5}},
  {{mux_sig_pin_2, 11}, {mux_sig_pin_2, 5}, {mux_sig_pin_1, 0}, {mux_sig_pin_1, 6}},
  {{mux_sig_pin_2, 10}, {mux_sig_pin_2, 4}, {mux_sig_pin_1, 1}, {mux_sig_pin_1, 7}},
};

#endif