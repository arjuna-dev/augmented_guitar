#ifndef DEVICE_SPECS_H
#define DEVICE_SPECS_H

constexpr int mux_1_pin = 30;
constexpr int mux_2_pin = 23;

constexpr int NUM_OF_STRINGS = 6;
constexpr int NUM_OF_FRETS = 4;
constexpr int NUM_OF_NOTES = 32;
constexpr int NUM_OF_MUX_PINS = 2;

const int value_positions[NUM_OF_STRINGS][NUM_OF_FRETS] = {
  {30, 18, 6, 5},
  {28, 16, 4, 7},
  {26, 14, 2, 9},
  {24, 12, 0, 11},
  {22, 10, 1, 13},
  {20, 8, 3, 15},
};

#endif