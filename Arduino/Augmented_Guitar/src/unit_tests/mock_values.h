#ifndef MOCK_VALUES_H
#define MOCK_VALUES_H

#include "../device_specs/device_specs.h"

extern int mux_pins_mock[NUM_OF_MUX_PINS];
extern int* ptr_mux_pins_mock;
extern int mux_ch_mock;
extern int* ptr_mux_ch_mock ;
extern int analog_values_mock[NUM_OF_NOTES];
extern int* ptr_analog_values_mock;

void selectMuxChannelMock(int mux_ch);

extern int tsi_mock_values_rand[NUM_OF_NOTES];

class Mocks {
  public:
    Mocks();
    int mock_array_rand[NUM_OF_NOTES];
    int mock_array_zeros[NUM_OF_NOTES];
    int mock_array_1000s[NUM_OF_NOTES];
    int mock_array_3000ish[NUM_OF_NOTES];
    int mock_array_20000ish[NUM_OF_NOTES];
    int* ptr_mock_values;
    void select_ptr_arr(int arr[]);
  private:
    void generate_array(int arr[], int size, int value);
    void generate_array_rand(int arr[], int size, int range);
};


extern Mocks mocks;


#endif
