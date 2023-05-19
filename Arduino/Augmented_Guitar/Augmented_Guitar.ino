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
#include "src/mux/mux.h"
#include "src/MIDI/midi.h"
#include "src/device_specs/device_specs.h"
#include "src/GuitarStringClass/guitar_string.h"

#define DEBUG 1

constexpr int string_input_pins[NUM_OF_STRINGS] = {34, 35, 36, 37, 38, 39};
constexpr char open_string_notes[NUM_OF_STRINGS] = {'E', 'A', 'D', 'G', 'B', 'e'};
constexpr int max_amplitudes[NUM_OF_STRINGS] = {540, 510, 560, 580, 620, 600};
constexpr int min_thresholds[NUM_OF_STRINGS] = {30, 40, 40, 40, 25, 43};
// constexpr int min_thresholds[NUM_OF_STRINGS] = {15, 18, 20, 20, 15, 23};
constexpr int max_wave_periods[NUM_OF_STRINGS] = {15, 10, 8, 6, 4, 3};

GuitarString guitar_strings[6];

void setup() {
  while (!Serial);

  /*_-_-GuitarString Class setup_-_-*/
  for (int i = 0; i < 6; i++) {
    guitar_strings[i] = {i, string_input_pins[i], open_string_notes[i], max_amplitudes[i], min_thresholds[i], max_wave_periods[i]};
  }

  /*_-_-Mux setup_-_-*/
  selectMuxChannel(0);
  for (int i = 0; i < 4; i++) {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], LOW);
  }

  /*_-_-Right hand setup_-_-*/
  for (int i = 0; i < 6; i++) {
    pinMode(string_input_pins[i], INPUT);
  }

}

void loop() {

  // aunit::TestRunner::run();

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_strings[i].updateStringMIDIValue();
  }

  // Detect peak and play MIDI for each string
  for (int i = 0; i < 6; i++) {
    guitar_strings[i].detect_note_on();
    guitar_strings[i].detect_note_off();
    guitar_strings[i].update_prev_and_current_amplitudes(analogRead);
  }

}
