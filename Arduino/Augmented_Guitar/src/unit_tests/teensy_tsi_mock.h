#ifndef TEENSY_TSI_MOCK_H
#define TEENSY_TSI_MOCK_H

#include "../teensy_touch/teensy_tsi_interface.h"
#include "../device_specs/device_specs.h"
#include "mock_values.h"

class TeensyTSIMock : public TeensyTSIInterface
{
  public:
    TeensyTSIMock(int* mock_TSI_values);
  private:
    int* _ptr_mock_TSI_values;
  protected:
    int teensyTouchInit(int pin) override;
    int teensyTouchDone() override;
    int teensyTouchReturn() override;
};


#endif
