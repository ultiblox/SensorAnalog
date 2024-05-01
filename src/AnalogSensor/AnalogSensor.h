#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Arduino.h"

class AnalogSensor {
  public:
    AnalogSensor(int pin);
    int read();
  private:
    int _pin;
};

#endif
