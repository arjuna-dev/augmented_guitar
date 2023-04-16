#ifndef READ_TOUCH_H
#define READ_TOUCH_H


class TeensyTouch 
{
  public:
    TeensyTouch(int pins_array[], int pins_array_size, int touch_values_array[], int touch_values_array_size);
    void readNonBlocking(int*& ptr_touch_values_array, int*& ptr_pin_values_array, int*& ptr_mux_ch_index);
  private:
    int teensyTouchInit(int pin);
    int teensyTouchDone();
    int teensyTouchReturn();
    int* _pins_array_first_value;
    int _pins_array_size;
    int* _touch_values_array_first_value;
    int _touch_values_array_size;
};


#endif
