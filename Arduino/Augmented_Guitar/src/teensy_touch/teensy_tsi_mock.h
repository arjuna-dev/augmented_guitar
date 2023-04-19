#ifndef TEENSY_TSI_MOCK_H
#define TEENSY_TSI_MOCK_H

#include "teensy_tsi_interface.h"
#include "../device_specs/device_specs.h"


extern int tsi_mock_values[NUM_OF_NOTES];
extern int* ptr_tsi_mock_values;

class TeensyTSIMock : public TeensyTSIInterface
{
  public:
    TeensyTSIMock();
  protected:
    int teensyTouchInit(int pin) override;
    int teensyTouchDone() override;
    int teensyTouchReturn() override;
  private:
    void load_to_array(int array[], int array_size);
};


#endif
