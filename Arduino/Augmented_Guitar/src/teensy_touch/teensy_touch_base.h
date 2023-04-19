#ifndef TEENSY_TOUCH_BASE_H
#define TEENSY_TOUCH_BASE_H

class TeensyTouchBase 
{
  public:
    TeensyTouchBase(int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size);
  protected:
    int teensyTouchInit(int pin);
    int teensyTouchDone();
    int teensyTouchReturn();
    int* _pins_array_first_value;
    int _pins_array_size;
    int* _touch_values_array_first_value;
    int _touch_values_array_size;
};


#endif
