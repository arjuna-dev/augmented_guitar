#ifndef TESTS_H
#define TESTS_H

#include "AUnit.h"
#include "../device_specs/device_specs.h"
#include "../teensy_touch/teensy_tsi_interface.h"
#include "teensy_tsi_mock.h"
#include "../teensy_touch/teensy_touch.h"

int mux_pins_mock[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins_mock = mux_pins_mock;
int mux_ch_mock = 0;
int* ptr_mux_ch_mock = &mux_ch_mock;

int touch_reference_analog_values_mock[NUM_OF_NOTES] = {0};
int* ptr_touch_reference_analog_values_mock = touch_reference_analog_values_mock;


TeensyTSIInterface* tsi_mock = new TeensyTSIMock();
TeensyTouch tt_reference_values_mock(tsi_mock, mux_pins_mock, 2, touch_reference_analog_values_mock, NUM_OF_NOTES);

void selectMuxChannelMock(int mux_ch) {}

test(TeensyTouch) {
  while (touch_reference_analog_values_mock[31] == 0) {
    tt_reference_values_mock.readNonBlocking(ptr_touch_reference_analog_values_mock, ptr_mux_pins_mock, ptr_mux_ch_mock, selectMuxChannelMock);
  }
  for (size_t i = 0; i < 32; i++) {
    assertEqual(touch_reference_analog_values_mock[i], tsi_mock_values[i]);
  }
}

test(incorrect) {
  int x = 1;
  assertNotEqual(x, 1);
}

#endif