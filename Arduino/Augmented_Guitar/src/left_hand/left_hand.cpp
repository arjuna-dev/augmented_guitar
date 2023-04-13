#include "Arduino.h"
#include "left_hand.h"
#include "../mux/mux.h"
#include "../MIDI/midi.h"
#include "../StringStruct/string_struct.h"
#include "../teensy_touch/teensy_touch.h"
#include "../device_specs/device_specs.h"

const int value_positions[NUM_OF_STRINGS][NUM_OF_FRETS] = {
  {30, 18, 6, 5},
  {28, 16, 4, 7},
  {26, 14, 2, 9},
  {24, 12, 0, 11},
  {22, 10, 1, 13},
  {20, 8, 3, 15},
};
const int capacitance_threshold = 5000;
int mux_pins[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins = mux_pins;
int touch_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_analog_values = touch_analog_values;
int mux_ch = 0;
int* ptr_mux_ch = &mux_ch;
int touch_reference_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_reference_analog_values = touch_reference_analog_values;

void setupLeftHand() {
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }
  // Calibration
  while (touch_reference_analog_values[31] == 0) {
    teensyTouchRead(touch_reference_analog_values, 32, ptr_touch_reference_analog_values, mux_pins, 2, ptr_mux_pins, ptr_mux_ch);
  }
}

void updateStringMIDIValue(struct StringStruct& string, int string_number) {
  bool is_last_fret_touched = true;
  bool is_fret_touched = false;
  bool any_fret_touched = false;
  for (int j = 0; j < 4; j++) {
    if (touch_analog_values[value_positions[string_number][j]] - touch_reference_analog_values[value_positions[string_number][j]] > capacitance_threshold) {
      is_fret_touched = true;
      any_fret_touched = true;
    } else {
      is_fret_touched = false;
    }
    if (is_last_fret_touched && is_fret_touched && !string.note_on) {
      string.pressed_fret = j + 1;
    }
    is_last_fret_touched = is_fret_touched;
  }

  if (!any_fret_touched) {
    string.pressed_fret = 0;
  }
  if (string.pressed_fret != string.last_sent_pressed_fret) {
    MIDI_lift_fret(string_number, string.last_sent_pressed_fret);
    MIDI_press_fret(string_number, string.pressed_fret);
    string.last_sent_pressed_fret = string.pressed_fret;
  }
}
