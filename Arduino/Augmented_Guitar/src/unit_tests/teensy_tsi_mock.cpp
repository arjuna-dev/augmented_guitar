#include "Arduino.h"
#include "teensy_tsi_mock.h"
#include "mock_values.h"

int TSI0_DATA_MOCK_VALUE = 0;


TeensyTSIMock::TeensyTSIMock(){}

int TeensyTSIMock::teensyTouchReturn() {
  delayMicroseconds(1);
  TSI0_DATA_MOCK_VALUE = *_ptr_mock_TSI_values;
  ptr_mock_TSI_values++;
  return TSI0_DATA_MOCK_VALUE;
}

int TeensyTSIMock::teensyTouchInit(int pin) {
  delayMicroseconds(10);
  return 0;
}

int TeensyTSIMock::teensyTouchDone() {
  return true;
}