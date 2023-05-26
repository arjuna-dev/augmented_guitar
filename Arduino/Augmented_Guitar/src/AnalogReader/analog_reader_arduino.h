#ifndef ANALOG_READER_ARDUINO_H
#define ANALOG_READER_ARDUINO_H

#include "analog_reader_interface.h"

class AnalogReaderArduino : public AnalogReaderInterface
{
  public:
    AnalogReaderArduino();
  protected:
    int analogReaderLeft(int pin) override;
    int analogReaderRight(int pin) override;
};


#endif
