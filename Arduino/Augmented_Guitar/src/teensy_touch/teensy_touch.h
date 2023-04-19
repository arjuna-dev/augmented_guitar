#ifndef TEENSY_TOUCH_H
#define TEENSY_TOUCH_H

#include "teensy_touch_base.h"


class TeensyTouch: public TeensyTouchBase {
  public:
    TeensyTouch(int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size);
    void readNonBlocking(int*& ptr_touch_values_array, int*& ptr_pin_values_array, int*& ptr_mux_ch_index);
};


#endif
