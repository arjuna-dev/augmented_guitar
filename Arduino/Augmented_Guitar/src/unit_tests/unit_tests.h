#ifndef TESTS_H
#define TESTS_H

#include <algorithm> 
#include "AUnit.h"
#include "../device_specs/device_specs.h"
#include "../teensy_touch/teensy_tsi_interface.h"
#include "teensy_tsi_mock.h"
#include "../teensy_touch/teensy_touch.h"
#include "mock_values.h"

// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_MOCKS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

TeensyTSIInterface* tsi_mock = new TeensyTSIMock();
TeensyTouch tt_reference_values_mock(tsi_mock, mux_pins_mock, 2, analog_values_mock, NUM_OF_NOTES);


// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_TESTS_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

test(TeensyTouchRand) {
  std::fill(analog_values_mock, analog_values_mock + NUM_OF_NOTES, 0);
  int* arr = mocks.mock_array_rand;
  mocks.select_ptr_arr(arr);
  while (analog_values_mock[31] == 0) {
    tt_reference_values_mock.readNonBlocking(ptr_analog_values_mock, ptr_mux_pins_mock, ptr_mux_ch_mock, selectMuxChannelMock);
  }
  for (size_t i = 0; i < 32; i++) {
    assertEqual(analog_values_mock[i], arr[i]);
  }
}

test(TeensyTouch1000) {
  std::fill(analog_values_mock, analog_values_mock + NUM_OF_NOTES, 0);
  int* arr = mocks.mock_array_1000s;
  mocks.select_ptr_arr(arr);
  while (analog_values_mock[31] == 0) {
    tt_reference_values_mock.readNonBlocking(ptr_analog_values_mock, ptr_mux_pins_mock, ptr_mux_ch_mock, selectMuxChannelMock);
  }
  for (size_t i = 0; i < 32; i++) {
    assertEqual(analog_values_mock[i], arr[i]);
  }
}

test(incorrect) {
  int x = 1;
  assertNotEqual(x, 1);
}

#endif