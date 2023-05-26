#ifndef ANALOG_READER_INTERFACE_H
#define ANALOG_READER_INTERFACE_H

class AnalogReaderInterface 
{
  public:
    virtual int analog_reader_left(int pin, int string) = 0;
    virtual int analog_reader_right(int pin) = 0;
};


#endif
