#include "Arduino.h"
#include "analog_reader_arduino.h"


AnalogReaderArduino::AnalogReaderArduino(){};

int AnalogReaderArduino::analogReaderLeft(int pin) {
    analogRead(pin);
}

int AnalogReaderArduino::analogReaderRight(int pin) {
    analogRead(pin);
}