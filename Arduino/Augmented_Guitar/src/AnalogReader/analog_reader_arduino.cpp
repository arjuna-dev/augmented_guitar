#include "Arduino.h"
#include "analog_reader_arduino.h"


AnalogReaderArduino::AnalogReaderArduino(){};

int AnalogReaderArduino::analog_reader_left(int pin) {
    analogRead(pin);
}

int AnalogReaderArduino::analog_reader_right(int pin) {
    analogRead(pin);
}