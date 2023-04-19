#ifndef TEENSY_TSI_H
#define TEENSY_TSI_H

#include "teensy_tsi_interface.h"

class TeensyTSI : public TeensyTSIInterface
{
  public:
    TeensyTSI();
  protected:
    int teensyTouchInit(int pin) override;
    int teensyTouchDone() override;
    int teensyTouchReturn() override;
};


#endif
