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
#include "../MIDI/midi_interface.h"
#include <limits.h>
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
    // _-_-_-_-_-RightHandFixtures_-_-_-_
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
    vector<AnalogReaderInterface*> analog_reader_mocks;
    vector<MIDIInterface*> midi_methods_mocks;
    vector<GuitarString> guitar_string_mocks;
    GuitarStringFriend guitar_string_friend;

    void initialize_all(){
      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        MIDIInterface* midi_methods_mock = new MIDIMethodsMock();
        midi_methods_mocks.push_back(midi_methods_mock); 
        guitar_string_mocks.push_back(GuitarString(analog_reader_mocks[i], midi_methods_mocks[i], i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i]));
      }
    }

    void custom_sine_setup(vector<int> _amplitude_mock_values_empty){
      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        AnalogReaderInterface* analog_reader_mock = new AnalogReaderMock(_amplitude_mock_values_empty, mock_pressed_frets_values, i);
        analog_reader_mocks.push_back(analog_reader_mock);
      }
      initialize_all();
    }

    void custom_sine_setup(vector<int>* _strings_sine_wave_mocks){
      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        AnalogReaderInterface* analog_reader_mock = new AnalogReaderMock(_strings_sine_wave_mocks[i], mock_pressed_frets_values, i);
        analog_reader_mocks.push_back(analog_reader_mock);
      }
      initialize_all();
    }

    void setup() override {
      TestOnce::setup();
    }

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

      for (int i = 0; i < NUM_OF_STRINGS; i++) {
        guitar_string_friend.set_note_on(guitar_string_mocks[i], false);
      }

      TestOnce::teardown();
    }
};

class RightHandEmptySineFixture: public GuitarStringFixture {
  protected:
    void setup() override {
      TestOnce::setup();
      custom_sine_setup(amplitude_mock_values_empty);
    }
};

class RightHandFullSineFixture: public GuitarStringFixture {
  protected:
    void setup() override {
      TestOnce::setup();
      custom_sine_setup(strings_sine_wave_mocks);
    }
};

class LeftHandFixture: public GuitarStringFixture {
  protected:
    void setup() override {
      TestOnce::setup();
      custom_sine_setup(amplitude_mock_values_empty);
    }
};


// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_TESTS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    // _-_-_-_-_-Right Hand-_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_


testF(RightHandFullSineFixture, detect_note_on){
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

testF(RightHandFullSineFixture, detect_note_on_MIDI){
  int vector_size = strings_sine_wave_mocks[0].size();
  bool detected_note_ons[NUM_OF_STRINGS] = {false};
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_friend.set_note_on(guitar_string_mocks[i], false);
  }
  for (int j = 0; j < vector_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      global_last_CC_mock = UNDEFINED;
      global_last_note_mock = 0;
      global_last_velocity_mock = -1;
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes();
      if (guitar_string_friend.get_note_on(guitar_string_mocks[i]) == true && detected_note_ons[i] == false) {
        detected_note_ons[i] = true;
        assertEqual(global_last_CC_mock, NOTE_ON);
        assertEqual(global_last_note_mock, MIDI_open_string_notes[i]);
        assertMore(global_last_velocity_mock, 0);
        assertLessOrEqual(global_last_velocity_mock, 127);
      }
    }
  }
}

testF(RightHandEmptySineFixture, detect_note_on_false_positive){
  int vector_size = strings_sine_wave_mocks[0].size();
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
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

testF(RightHandFullSineFixture, detect_note_off_through_time){
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

testF(RightHandFullSineFixture, detect_note_off_through_amplitude){
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

testF(RightHandFullSineFixture, detect_note_off_through_time_false_positive){
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

testF(RightHandFullSineFixture, detect_note_off_false_positive_through_amplitude){
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

int max_ADC_value = 1024;

testF(LeftHandFixture, detect_all_frets_pressed){

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    for (int j = 0; j < NUM_OF_FRETS; j++) {
      mock_pressed_frets_values[i][j] = max_ADC_value;
    }
  }

  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].update_string_MIDI_value();
  }
  for (int i = 0; i < 6; i++) {
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

testF(LeftHandFixture, detect_all_frets_pressed_above_max_ADC_value){

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    for (int j = 0; j < NUM_OF_FRETS; j++) {
      mock_pressed_frets_values[i][j] = random(max_ADC_value, INT_MAX);
    }
  }

  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].update_string_MIDI_value();
  }
  for (int i = 0; i < 6; i++) {
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

testF(LeftHandFixture, detect_each_fret_above_boundary){

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    for (int j = 0; j < NUM_OF_FRETS; j++) {
      mock_pressed_frets_values[i][j] = fret_touched_threshold+1;
      guitar_string_mocks[i].update_string_MIDI_value();
      assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + j + 1);
    }
  }
}

testF(LeftHandFixture, detect_each_fret_below_boundary){

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    for (int j = 0; j < NUM_OF_FRETS; j++) {
      mock_pressed_frets_values[i][j] = fret_touched_threshold-1;
      guitar_string_mocks[i].update_string_MIDI_value();
      assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i]);
    }
  }
}


testF(LeftHandFixture, detect_each_fret_MIDI){
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    for (int j = 0; j < NUM_OF_FRETS; j++) {
      mock_pressed_frets_values[i][j] = fret_touched_threshold+random(max_ADC_value-fret_touched_threshold);
      guitar_string_mocks[i].update_string_MIDI_value();
      assertEqual(global_last_CC_mock, PRESS_FRET);
      assertEqual(global_last_note_mock, MIDI_open_string_notes[i] + j + 1);
      assertEqual(global_last_velocity_mock, 0);
      mock_pressed_frets_values[i][j] = fret_touched_threshold-random(fret_touched_threshold-1);
      guitar_string_mocks[i].update_string_MIDI_value();
      assertEqual(global_last_CC_mock, PRESS_FRET);
      assertEqual(global_last_note_mock, MIDI_open_string_notes[i]);
      assertEqual(global_last_velocity_mock, 0);
    }
  }
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    mock_pressed_frets_values[i][3] = 1000;
    guitar_string_mocks[i].update_string_MIDI_value();
  }
}

#endif