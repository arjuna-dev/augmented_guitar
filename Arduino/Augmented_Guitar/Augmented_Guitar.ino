/*
  Hexaphonic Pickup Color Coding
  brown  :E string
  yellow :A string
  green  :D string
  red    :G string
  orange :B string
  white  :E string
*/

#include "src/unit_tests/unit_tests.h"

#include "src/debug/debug.h"
#include "src/teensy_touch/teensy_tsi_interface.h"
#include "src/teensy_touch/teensy_tsi.h"
#include "src/teensy_touch/teensy_touch.h"
#include "src/mux/mux.h"
#include "src/MIDI/midi.h"
#include "src/device_specs/device_specs.h"
#include "src/GuitarStringClass/guitar_string.h"

#define DEBUG 1

const int string_input_pins[NUM_OF_STRINGS] = {34, 35, 36, 37, 38, 39};
const char open_string_notes[NUM_OF_STRINGS] = {'E', 'A', 'D', 'G', 'B', 'e'};
const int max_amplitudes[NUM_OF_STRINGS] = {540, 510, 560, 580, 620, 600};
const int min_thresholds[NUM_OF_STRINGS] = {15, 18, 20, 20, 15, 23};
const int max_wave_periods[NUM_OF_STRINGS] = {15, 10, 8, 6, 4, 3};

int mux_pins[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins = mux_pins;
int touch_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_analog_values = touch_analog_values;
int mux_ch = 0;
int* ptr_mux_ch = &mux_ch;
int touch_reference_analog_values[NUM_OF_NOTES] = {0};
int* ptr_touch_reference_analog_values = touch_reference_analog_values;


TeensyTSIInterface* tsi = new TeensyTSI();

TeensyTouch tt_reference_values(tsi, mux_pins, 2, touch_reference_analog_values, NUM_OF_NOTES);
TeensyTouch tt_analog_values(tsi, mux_pins, 2, touch_analog_values, NUM_OF_NOTES);

GuitarString guitar_strings[6];

void setup() {
#if DEBUG == 1
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial);
#endif

  /*_-_-Class setup_-_-*/
  for (int i = 0; i < 6; i++) {
    guitar_strings[i] = {i, string_input_pins[i], open_string_notes[i], max_amplitudes[i], min_thresholds[i], max_wave_periods[i], touch_analog_values, touch_reference_analog_values};
  }

  /*_-_-Left hand setup_-_-*/
  selectMuxChannel(0);
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }

  /*_-_-Right hand setup_-_-*/
  for (int i = 0; i < 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }

  // Capacitance calibration
  while (touch_reference_analog_values[31] == 0) {
    tt_reference_values.readNonBlocking(ptr_touch_reference_analog_values, ptr_mux_pins, ptr_mux_ch, selectMuxChannel);
  }

}

void loop() {

  tt_analog_values.readNonBlocking(ptr_touch_analog_values, ptr_mux_pins, ptr_mux_ch, selectMuxChannel);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_strings[i].updateStringMIDIValue();
  }

  // Detect peak and play MIDI for each string
  for (int i = 0; i < 6; i++) {
    guitar_strings[i].detect_note_on_off();
  }

  aunit::TestRunner::run();
}
