#ifndef TEENSY_TOUCH_H
#define TEENSY_TOUCH_H

#include "teensy_tsi_interface.h"


class TeensyTouch {
  public:
    TeensyTouch(TeensyTSIInterface* tsi_interface, int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size);
    void readNonBlocking(int*& ptr_touch_values_array, int*& ptr_pin_values_array, int*& ptr_mux_ch_index, void (*mux_function)(int));
  private:
    TeensyTSIInterface* _tsi_interface;
    int* _pins_array_first_value;
    int _pins_array_size;
    int* _touch_values_array_first_value;
    int _touch_values_array_size; 
};


#endif
