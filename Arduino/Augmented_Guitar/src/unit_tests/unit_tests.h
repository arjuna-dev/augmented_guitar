#ifndef TESTS_H
#define TESTS_H

#include "Arduino.h"

#include <algorithm> 
#include "AUnit.h"
#include "../device_specs/device_specs.h"
#include "../teensy_touch/teensy_tsi_interface.h"
#include "teensy_tsi_mock.h"
#include "../teensy_touch/teensy_touch.h"
#include "mock_values.h"
#include "../guitarStringClass/guitar_string.h"


// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_MOCKS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

TeensyTSIInterface* tsi_mock = new TeensyTSIMock();
TeensyTouch tt_mock(tsi_mock, mux_pins_mock, 2, analog_values_mock, NUM_OF_NOTES);

GuitarString guitar_string_mocks[6];

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

class GuitarStringFixture: public aunit::TestOnce {
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

testF(GuitarStringFixture, basic_guitar_string_test){
  ptr_mock_amplitude_values = amplitude_mock_values_e;
  int values_size = 1000;
  for (int i = 0; i < values_size; i++) {
    guitar_string_mocks[0].detect_note_on_off(analogReadMock);
    if (guitar_string_mocks[0]._note_on) {
      pass();
      return;
    } 
  }
  fail();
}
#endif