#ifndef TEENSY_TSI_INTERFACE_H
#define TEENSY_TSI_INTERFACE_H

class TeensyTSIInterface 
{
  public:
    virtual int teensyTouchInit(int pin) = 0;
    virtual int teensyTouchDone() = 0;
    virtual int teensyTouchReturn() = 0;
};


#endif
