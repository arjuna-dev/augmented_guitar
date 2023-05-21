#ifndef TESTS_H
#define TESTS_H

#include "Arduino.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include "AUnit.h"
#include "../device_specs/device_specs.h"
#include "mock_values.h"
#include "../guitarStringClass/guitar_string_testable.h"
#include "../MIDI/midi.h"


// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-FIXTURES-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void populate_array(int arr[], int size, int value) {
  for (int i=0; i<size; i++) {
      arr[i] = value;
  }
}

void populate_array_rand(int arr[], int size, int min = 0, int max = 0) {
  if (min != 0 || max != 0) {
    int range = max - min + 1;
    for (int i=0; i<size; i++) {
        arr[i] = (rand()% range) + min;
    }
  } else {
    for (int i=0; i<size; i++) {
        arr[i] = rand();
    }
  }
}

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
    // _-_-_-_GuitarStringsFixture_-_-_-_
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

class RightHandFixture: public aunit::TestOnce {
  protected:
    GuitarStringTestable guitar_string_mocks[6];
    void setup() override {
      TestOnce::setup();
      for (int i = 0; i < 6; i++) {
        guitar_string_mocks[i] = {i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i], strings_sine_wave_mocks[i]};
      }
    }

    void teardown() override {
      // Teardown code here
      TestOnce::teardown();
    }

};

class LeftHandFixture: public aunit::TestOnce {
  protected:
    GuitarStringTestable guitar_string_mocks[6];
    void setup() override {
      TestOnce::setup();
      for (int i = 0; i < 6; i++) {
        guitar_string_mocks[i] = {i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i], strings_sine_wave_mocks[i]};
      }
    }

    void teardown() override {
      // Teardown code here
      TestOnce::teardown();
    }
};

// // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// // _-_-_-_-_-_-_-_-_-_-_-_-_TESTS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    // _-_-_-_-_-Right Hand-_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

testF(RightHandFixture, detect_note_on){
  int vector_size = guitar_string_mocks[0]._mock_sine_wave_vector.size();
  bool detected_note_ons[NUM_OF_STRINGS] = {false};
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(false);
  }
  for (int j = 0; j < vector_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes();
      if (guitar_string_mocks[i].get_note_on() == true && detected_note_ons[i] == false) {
        detected_note_ons[i] = true;
        int sum = std::accumulate(detected_note_ons, detected_note_ons + NUM_OF_STRINGS, 0);
        if (sum == NUM_OF_STRINGS) {
          pass();
          return;
        }
      }
    }
  }
  fail();
}

testF(RightHandFixture, detect_note_on_false_positive){
  int vector_size = guitar_string_mocks[0]._mock_sine_wave_vector.size();
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    std::fill(guitar_string_mocks[i]._mock_sine_wave_vector.begin(), guitar_string_mocks[i]._mock_sine_wave_vector.end(), 0);
    guitar_string_mocks[i].set_note_on(false);
  }
  for (int j = 0; j < vector_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes();
      if (guitar_string_mocks[i].get_note_on() == true) {
          fail();
          return;
      }
    }
  }
  pass();
}

testF(RightHandFixture, detect_note_off_through_time){
  int iterations = 500;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].set_note_on_timestamp(millis()-max_wave_periods_mock[i]-1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_mocks[i].get_note_on() == false && detected_note_offs[i] == false) {
        detected_note_offs[i] = true;
        int sum = std::accumulate(detected_note_offs, detected_note_offs + NUM_OF_STRINGS, 0);
        if (sum == NUM_OF_STRINGS) {
          pass();
          return;
        }
      }
    }
  }
  fail();
}

testF(RightHandFixture, detect_note_off_through_amplitude){
  int iterations = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].set_current_amplitude(min_thresholds_mock[i]-1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_mocks[i].get_note_on() == false && detected_note_offs[i] == false) {
        detected_note_offs[i] = true;
        int sum = std::accumulate(detected_note_offs, detected_note_offs + NUM_OF_STRINGS, 0);
        if (sum == NUM_OF_STRINGS) {
          pass();
          return;
        }
      }
    }
  }
  fail();
}

testF(RightHandFixture, detect_note_off_through_time_false_positive){
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }

  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].set_note_on_timestamp(millis()-max_wave_periods_mock[i]+1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_mocks[i].get_note_on() == false) {
          fail();
          return;
      }
    }
  }
  pass();
}

testF(RightHandFixture, detect_note_off_false_positive_through_amplitude){
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }

  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].set_current_amplitude(min_thresholds_mock[i]+1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_mocks[i].get_note_on() == false) {
          fail();
          return;
      }
    }
  }
  pass();
}

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    // _-_-_-_-_-Left Hand-_-_-_-_-_-_
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

testF(LeftHandFixture, detect_finger_position_all_frets_pressed){

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].updateStringMIDIValue();
  }
  for (int i = 0; i < 6; i++) {
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

#endif