#include "Arduino.h"
#include "teensy_tsi_mock.h"

int TSI0_DATA_MOCK = 0;

int tsi_mock_values[NUM_OF_NOTES]  = {0};
int* ptr_tsi_mock_values = tsi_mock_values;


TeensyTSIMock::TeensyTSIMock(){
  load_to_array(tsi_mock_values, NUM_OF_NOTES);
}

void TeensyTSIMock::load_to_array(int array[], int array_size) {
  for (int i = 0; i < array_size; i++) {
    array[i] =  rand();
  }
}




int TeensyTSIMock::teensyTouchReturn() {
  delayMicroseconds(1);
  TSI0_DATA_MOCK = *ptr_tsi_mock_values;
  ptr_tsi_mock_values++;
  return TSI0_DATA_MOCK;
}

int TeensyTSIMock::teensyTouchInit(int pin) {
  delayMicroseconds(10);
  return 0;
}

int TeensyTSIMock::teensyTouchDone() {
  return true;
}