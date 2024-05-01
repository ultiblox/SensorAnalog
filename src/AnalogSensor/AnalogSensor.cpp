#include "AnalogSensor.h"
#include "Arduino.h"

AnalogSensor::AnalogSensor(int pin) {
  _pin = pin;
  pinMode(_pin, INPUT);
}

int AnalogSensor::read() {
  return analogRead(_pin);
}
