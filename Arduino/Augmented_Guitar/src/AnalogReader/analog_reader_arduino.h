#ifndef ANALOG_READER_ARDUINO_H
#define ANALOG_READER_ARDUINO_H

#include "analog_reader_interface.h"

class AnalogReaderArduino : public AnalogReaderInterface
{
  public:
    AnalogReaderArduino();
  protected:
    int analog_reader_left(int pin) override;
    int analog_reader_right(int pin) override;
};


#endif
