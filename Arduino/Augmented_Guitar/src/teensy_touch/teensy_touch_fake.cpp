#include "Arduino.h"
#include "core_pins.h"
#include "../mux/mux.h"
#include "teensy_touch_fake.h"


TeensyTouchFake::TeensyTouchFake(int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size) : TeensyTouchBase(pins_array, pins_array_size, touch_values_array, touch_values_array_size) {
  teensyTouchInit(*pins_array);
}


void TeensyTouchFake::readNonBlocking(int*& ptr_touch_array, int*& ptr_pin_array, int*& ptr_mux_ch_index) {
  Serial.println("TeensyTouchFake::readNonBlocking() called");
}
