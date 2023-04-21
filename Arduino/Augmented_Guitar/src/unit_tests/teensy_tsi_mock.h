#ifndef TEENSY_TSI_MOCK_H
#define TEENSY_TSI_MOCK_H

#include "../teensy_touch/teensy_tsi_interface.h"
#include "../device_specs/device_specs.h"
#include "mock_values.h"

extern int* ptr_tsi_mock_values;

class TeensyTSIMock : public TeensyTSIInterface
{
  public:
    TeensyTSIMock();
  protected:
    int teensyTouchInit(int pin) override;
    int teensyTouchDone() override;
    int teensyTouchReturn() override;
};


#endif
