#ifndef TESTS_H
#define TESTS_H

#include "AUnit.h"
#include "Arduino.h"
#include "mock_values.h"
#include "../device_specs/device_specs.h"
#include "../GuitarStringClass/guitar_string.h"
#include "../GuitarStringClass/guitar_string_friend.h"
#include "../MIDI/midi_methods_mock.h"
#include "../MIDI/midi_interface.h"
#include "../AnalogReader/analog_reader_interface.h"
#include "../AnalogReader/analog_reader_mock.h"
#include <algorithm>
#include <numeric>
#include <vector>

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

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-FIXTURES-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
    // _-_-_-_GuitarStringsFixture_-_-_-_
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

class GuitarStringFixture: public aunit::TestOnce {
  protected:
    vector<vector<int>> mock_pressed_frets_values = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    };
    MIDIInterface* midi_methods_mock = new MIDIMethodsMock();
    vector<AnalogReaderInterface*> analog_reader_mocks;
    vector<MIDIInterface*> midi_methods_mocks;
    vector<GuitarString> guitar_string_mocks;
    GuitarStringFriend guitar_string_friend;
    vector<int> _strings_sine_wave_mocks[NUM_OF_STRINGS];
    void setup() override {
      TestOnce::setup();

    void teardown() override {
      // delete all dynamically allocated objects
      for (auto mock : analog_reader_mocks) {
        delete mock;
      }
      for (auto mock : midi_methods_mocks) {
        delete mock;
      }
      // clear the vector for the next setup
      analog_reader_mocks.clear();
      midi_methods_mocks.clear();
      guitar_string_mocks.clear();
      TestOnce::teardown();
    }
      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        analog_reader_mocks.push_back(analog_reader_mock);
        midi_methods_mocks.push_back(midi_methods_mock); 
      }
      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        AnalogReaderInterface* analog_reader_mock = new AnalogReaderMock(_strings_sine_wave_mocks[i], mock_pressed_frets_values, i);
        guitar_string_mocks.push_back(GuitarString(analog_reader_mock, midi_methods_mock, i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i]));
        analog_reader_mocks.push_back(analog_reader_mock);
        midi_methods_mocks.push_back(midi_methods_mock); 
      }
    }

        analog_reader_mocks.push_back(analog_reader_mock);
        midi_methods_mocks.push_back(midi_methods_mock); 
};

// // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// // _-_-_-_-_-_-_-_-_-_-_-_-_TESTS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    // _-_-_-_-_-Right Hand-_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

testF(GuitarStringFixture, detect_note_on){
  int vector_size = strings_sine_wave_mocks[0].size();
  bool detected_note_ons[NUM_OF_STRINGS] = {false};
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], false);
  }
  for (int j = 0; j < vector_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes();
      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == true && detected_note_ons[i] == false) {
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

testF(GuitarStringFixture, detect_note_on_false_positive){
  int vector_size = strings_sine_wave_mocks[0].size();
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    std::fill(_strings_sine_wave_mocks[i].begin(), _strings_sine_wave_mocks[i].end(), 0);
    guitar_string_friend.set_note_on(guitar_string_mocks[i], false);
  }
  for (int j = 0; j < vector_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes();
      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == true) {
          fail();
          return;
      }
    }
  }
  pass();
}

testF(GuitarStringFixture, detect_note_off_through_time){
  int iterations = 500;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_friend.set_note_on_timestamp(guitar_string_mocks[i], millis()-max_wave_periods_mock[i]-1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == false && detected_note_offs[i] == false) {
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

testF(GuitarStringFixture, detect_note_off_through_amplitude){
  int iterations = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < iterations; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_friend.set_current_amplitude(guitar_string_mocks[i], min_thresholds_mock[i]-1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == false && detected_note_offs[i] == false) {
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

testF(GuitarStringFixture, detect_note_off_through_time_false_positive){
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], true);
  }

  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_friend.set_note_on_timestamp(guitar_string_mocks[i], millis()-max_wave_periods_mock[i]+1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == false) {
          fail();
          return;
      }
    }
  }
  pass();
}

testF(GuitarStringFixture, detect_note_off_false_positive_through_amplitude){
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], true);
  }

  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_friend.set_current_amplitude(guitar_string_mocks[i], min_thresholds_mock[i]+1);
      guitar_string_mocks[i].detect_note_off();

      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == false) {
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

testF(GuitarStringFixture, detect_finger_position_all_frets_pressed){

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    mock_pressed_frets_values[i][3] = 1000;
  }

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].update_string_MIDI_value();
  }
  for (int i = 0; i < 6; i++) {
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

#endif