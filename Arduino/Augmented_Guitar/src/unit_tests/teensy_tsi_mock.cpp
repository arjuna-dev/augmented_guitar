#include "Arduino.h"
#include "teensy_tsi_mock.h"
#include "mock_values.h"

int TSI0_DATA_MOCK = 0;


TeensyTSIMock::TeensyTSIMock(){}

int TeensyTSIMock::teensyTouchReturn() {
  delayMicroseconds(1);
  TSI0_DATA_MOCK = *ptr_mock_TSI_values;
  ptr_mock_TSI_values++;
  return TSI0_DATA_MOCK;
}

int TeensyTSIMock::teensyTouchInit(int pin) {
  delayMicroseconds(10);
  return 0;
}

int TeensyTSIMock::teensyTouchDone() {
  return true;
}