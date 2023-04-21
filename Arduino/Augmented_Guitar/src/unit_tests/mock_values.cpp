#include <cstdlib>
#include "mock_values.h"

int mux_pins_mock[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins_mock = mux_pins_mock;
int mux_ch_mock = 0;
int* ptr_mux_ch_mock = &mux_ch_mock;
int analog_values_mock[NUM_OF_NOTES] = {0};
int* ptr_analog_values_mock = analog_values_mock;

void selectMuxChannelMock(int mux_ch) {}

Mocks::Mocks() {
  generate_array_rand(mock_array_rand, NUM_OF_NOTES, 1);
  generate_array(mock_array_zeros, NUM_OF_NOTES, 0);
  generate_array(mock_array_1000s, NUM_OF_NOTES, 1000);
  generate_array(mock_array_3000ish, NUM_OF_NOTES, 3000);
  generate_array(mock_array_20000ish, NUM_OF_NOTES, 20000);
  select_ptr_arr(mock_array_rand);
}

void Mocks::generate_array(int arr[], int size, int value) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = value;
    }
}

void Mocks::generate_array_rand(int arr[], int size, int range) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand();
    }
}

void Mocks::select_ptr_arr(int arr[]){
  ptr_mock_values = arr;
}


Mocks mocks;