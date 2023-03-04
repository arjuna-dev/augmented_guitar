#ifndef READ_TOUCH_H
#define READ_TOUCH_H

int teensyTouchInit(uint8_t pin);
bool teensyTouchDone();
int teensyTouchReturn();
void teensyTouchRead(int touch_array[], int touch_array_size, int*& ptr_touch_array, int pin_array[],
                     int pin_array_size, int*& ptr_pin_array, int*& ptr_mux_ch_index, int* ptr_mux_ch_original);                     

#endif
