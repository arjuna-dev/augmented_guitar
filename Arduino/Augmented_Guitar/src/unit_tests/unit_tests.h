#ifndef TESTS_H
#define TESTS_H

#include "Arduino.h"

#include <numeric>
#include "AUnit.h"
#include "../device_specs/device_specs.h"
#include "../teensy_touch/teensy_tsi_interface.h"
#include "teensy_tsi_mock.h"
#include "../teensy_touch/teensy_touch.h"
#include "mock_values.h"
#include "../guitarStringClass/guitar_string_testable.h"
#include "../MIDI/midi.h"


// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_MOCKS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

TeensyTSIInterface* tsi_mock = new TeensyTSIMock();
TeensyTouch tt_mock(tsi_mock, mux_pins_mock, 2, analog_values_mock, NUM_OF_NOTES);

GuitarStringTestable guitar_string_mocks[6];

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-FIXTURES-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
    // _-_-_-_-_-_-TeensyTouch-_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

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

class TTFixture: public aunit::TestOnce {
  protected:
    void setup() override {
      TestOnce::setup();
      std::fill(analog_values_mock, analog_values_mock + NUM_OF_NOTES, 0);
    }

    void teardown() override {
      std::fill(analog_values_mock, analog_values_mock + NUM_OF_NOTES, 0);
      TestOnce::teardown();
    }

    void setup_arr_and_ptr(int arr[], int size, int value) {
      populate_array(arr, size, value);
      ptr_mock_TSI_values = arr;
    }

    void setup_arr_and_ptr_rand(int arr[], int size, int min = 0, int max = 0) {
      populate_array_rand(arr, size, min, max);
      ptr_mock_TSI_values = arr;
    }

};

    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
    // _-_-_-_-_-_GuitarStrings_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

class RightHandFixture: public aunit::TestOnce {
  protected:
    int empty_mock_arr[1000] = {0};
    void setup() override {
      TestOnce::setup();
      ptr_amplitude_mock_values_E = empty_mock_arr;
      ptr_amplitude_mock_values_A = empty_mock_arr;
      ptr_amplitude_mock_values_D = empty_mock_arr;
      ptr_amplitude_mock_values_G = empty_mock_arr;
      ptr_amplitude_mock_values_B = empty_mock_arr;
      ptr_amplitude_mock_values_e = empty_mock_arr;
      for (int i = 0; i < 6; i++) {
        guitar_string_mocks[i] = {i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i], analog_values_mock, reference_analog_values_mock};
      }
    }

    void teardown() override {
      ptr_amplitude_mock_values_E = empty_mock_arr;
      ptr_amplitude_mock_values_A = empty_mock_arr;
      ptr_amplitude_mock_values_D = empty_mock_arr;
      ptr_amplitude_mock_values_G = empty_mock_arr;
      ptr_amplitude_mock_values_B = empty_mock_arr;
      ptr_amplitude_mock_values_e = empty_mock_arr;
      TestOnce::teardown();
    }

};
  protected:
    void setup() override {
      TestOnce::setup();
      for (int i = 0; i < 6; i++) {
        guitar_string_mocks[i] = {i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i], analog_values_mock, reference_analog_values_mock};
      }
    }

    void teardown() override {
      TestOnce::teardown();
    }

};

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_TESTS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

testF(TTFixture, teensy_touch_rand){
  int array_rand[NUM_OF_NOTES] = {0};
  setup_arr_and_ptr_rand(array_rand, NUM_OF_NOTES);

  while (analog_values_mock[31] == 0) {
    tt_mock.readNonBlocking(ptr_analog_values_mock, ptr_mux_pins_mock, ptr_mux_ch_mock, selectMuxChannelMock);
  }
  for (size_t i = 0; i < 32; i++) {
    assertEqual(analog_values_mock[i], array_rand[i]);
  }
}

testF(TTFixture, teensy_touch_1000s){
  int array_1000s[NUM_OF_NOTES] = {0};
  setup_arr_and_ptr(array_1000s, NUM_OF_NOTES, 1000);

  while (analog_values_mock[31] == 0) {
    tt_mock.readNonBlocking(ptr_analog_values_mock, ptr_mux_pins_mock, ptr_mux_ch_mock, selectMuxChannelMock);
  }
  for (size_t i = 0; i < 32; i++) {
    assertEqual(analog_values_mock[i], array_1000s[i]);
  }
}
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    // _-_-_-_-_-Right Hand-_-_-_-_-_-
    // _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

testF(RightHandFixture, detect_note_on){
  int values_size = 1000;
  // TODO: Get graphs for E and D strings
  ptr_amplitude_mock_values_E = amplitude_mock_values_A;
  ptr_amplitude_mock_values_A = amplitude_mock_values_A;
  ptr_amplitude_mock_values_D = amplitude_mock_values_G;
  ptr_amplitude_mock_values_G = amplitude_mock_values_G;
  ptr_amplitude_mock_values_B = amplitude_mock_values_B;
  ptr_amplitude_mock_values_e = amplitude_mock_values_e;
  bool detected_note_ons[NUM_OF_STRINGS] = {false};
  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(false);
  }
  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes(analogReadMock);
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
  int values_size = 1000;
  int empty_array[values_size] = {0};
  ptr_amplitude_mock_values_E = empty_array;
  ptr_amplitude_mock_values_A = empty_array;
  ptr_amplitude_mock_values_D = empty_array;
  ptr_amplitude_mock_values_G = empty_array;
  ptr_amplitude_mock_values_B = empty_array;
  ptr_amplitude_mock_values_e = empty_array;
  guitar_string_mocks[0].set_note_on(false);
  for (int j = 0; j < values_size; j++) {
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
      guitar_string_mocks[i].detect_note_on();
      guitar_string_mocks[i].update_prev_and_current_amplitudes(analogReadMock);
      if (guitar_string_mocks[i].get_note_on() == true) {
          fail();
          return;
      }
    }
  }
  pass();
}

testF(RightHandFixture, detect_note_off){
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < values_size; j++) {
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
  int values_size = 1000;

  for (int i = 0; i < NUM_OF_STRINGS; i++) {
    guitar_string_mocks[i].set_note_on(true);
  }
  bool detected_note_offs[NUM_OF_STRINGS] = {false};
  for (int j = 0; j < values_size; j++) {
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

testF(RightHandFixture, detect_note_off_false_positive){
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

testF(GuitarStringFixture, detect_finger_position_all_frets_pressed){
  populate_array(reference_analog_values_mock, NUM_OF_NOTES, 3000);
  populate_array(analog_values_mock, NUM_OF_NOTES, 20000);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].updateStringMIDIValue();
  }
  for (int i = 0; i < 6; i++) {
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

#endif