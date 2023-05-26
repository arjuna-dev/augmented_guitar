#ifndef ANALOG_READER_INTERFACE_H
#define ANALOG_READER_INTERFACE_H

class AnalogReaderInterface 
{
  public:
    virtual int analogReaderLeft(int pin) = 0;
    virtual int analogReaderRight(int pin) = 0;
};


#endif
