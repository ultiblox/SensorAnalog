#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Arduino.h"

class AnalogSensor {
public:
    AnalogSensor(int pin); // Constructor initializes the pin
    AnalogSensor& setInterval(unsigned long interval); // Sets the reading interval
    AnalogSensor& setCallback(void (*callback)(int)); // Sets the callback function
    int read(); // Returns the immediate sensor value
    void loop(); // Checks interval and triggers callback if needed
private:
    int _pin;
    unsigned long _interval;
    unsigned long _lastReadTime;
    int _lastReadValue;
    void (*_callback)(int); // Pointer to the callback function
};

#endif
