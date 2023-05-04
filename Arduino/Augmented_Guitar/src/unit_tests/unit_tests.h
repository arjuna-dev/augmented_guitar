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

testF(GuitarStringFixture, detect_note_on){
  ptr_mock_amplitude_values = amplitude_mock_values_e;
  int values_size = 1000;
  guitar_string_mocks[0].set_note_on(false);
  for (int i = 0; i < values_size; i++) {
    guitar_string_mocks[0].detect_note_on();
    guitar_string_mocks[0].update_prev_and_current_amplitudes(analogReadMock);
    if (guitar_string_mocks[0].get_note_on() == true) {
      pass();
      return;
    }
  }
  fail();
}

testF(GuitarStringFixture, detect_note_on_false_positive){
  ptr_mock_amplitude_values = amplitude_mock_values_e_empty;
  int values_size = 1000;
  guitar_string_mocks[0].set_note_on(false);
  for (int i = 0; i < values_size; i++) {
    guitar_string_mocks[0].detect_note_on();
    guitar_string_mocks[0].update_prev_and_current_amplitudes(analogReadMock);
    if (guitar_string_mocks[0].get_note_on() == true) {
      fail();
      return;
    }
  }
  pass();
}

testF(GuitarStringFixture, detect_note_off_false_positive){
  ptr_mock_amplitude_values = amplitude_mock_values_e;
  int values_size = 1000;

  guitar_string_mocks[0].set_note_on(true);
  guitar_string_mocks[0].set_note_on_timestamp();

  for (int i = 0; i < values_size; i++) {
    guitar_string_mocks[0].detect_note_off();
    guitar_string_mocks[0].update_prev_and_current_amplitudes(analogReadMock);
    if (guitar_string_mocks[0].get_note_on() == false) {
      Serial.println("Error: note off detected when note on");
      fail();
      return;
    }
  }
  pass();
}

testF(GuitarStringFixture, detect_note_off){
  ptr_mock_amplitude_values = amplitude_mock_values_e_note_off;
  int values_size = 1000;
  guitar_string_mocks[0].set_note_on(true);
  for (int i = 0; i < values_size; i++) {
    guitar_string_mocks[0].detect_note_on();
    guitar_string_mocks[0].detect_note_off();
    guitar_string_mocks[0].update_prev_and_current_amplitudes(analogReadMock);
    if (guitar_string_mocks[0].get_note_on() == false) {
      pass();
      return;
    }
  }
  fail();
}

testF(GuitarStringFixture, detect_finger_position_all_frets_pressed){
  populate_array(reference_analog_values_mock, NUM_OF_NOTES, 3000);
  populate_array(analog_values_mock, NUM_OF_NOTES, 20000);

  // Update the MIDI values according to pressed frets
  for (int i = 0; i < 6; i++) {
    guitar_string_mocks[i].updateStringMIDIValue();
  }
  for (int i = 0; i < 6; i++) {
    Serial.println(guitar_string_mocks[i].get_MIDI_value());
    assertEqual(guitar_string_mocks[i].get_MIDI_value(), MIDI_open_string_notes[i] + NUM_OF_FRETS);
  }
}

#endif