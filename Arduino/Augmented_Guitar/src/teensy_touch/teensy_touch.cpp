#include "Arduino.h"






#include "core_pins.h"
#include "../mux/mux.h"
#include "teensy_touch.h"


TeensyTouch::TeensyTouch(TeensyTSIInterface* tsi_interface, int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size) : _tsi_interface(tsi_interface), _pins_array_first_value(pins_array), _pins_array_size(pins_array_size), _touch_values_array_first_value(touch_values_array), _touch_values_array_size(touch_values_array_size) {
  _tsi_interface->teensyTouchInit (*_pins_array_first_value);
}

void TeensyTouch::readNonBlocking(int*& ptr_touch_array, int*& ptr_pin_array, int*& ptr_mux_ch_index, void (*mux_function)(int)) {

  if (_tsi_interface->teensyTouchDone()) {

    int* touch_array_end = _touch_values_array_first_value + _touch_values_array_size;
    int* pin_array_end = _pins_array_first_value + _pins_array_size;

    *ptr_touch_array = _tsi_interface->teensyTouchReturn();
    ptr_touch_array++;

    if (ptr_touch_array == touch_array_end) {
      ptr_touch_array = _touch_values_array_first_value;
    }
    ptr_pin_array++;


    if (ptr_pin_array == pin_array_end) {
      ptr_pin_array = _pins_array_first_value;
      (*ptr_mux_ch_index)++;
      if (*ptr_mux_ch_index == 16) {
        *ptr_mux_ch_index = 0;
      }
      mux_function(*ptr_mux_ch_index);
    }
    int current_pin = *ptr_pin_array;
    _tsi_interface->teensyTouchInit(current_pin);
  }
}
