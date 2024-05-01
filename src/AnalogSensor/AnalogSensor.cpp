#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(int pin) : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr) {
    pinMode(_pin, INPUT);
}

AnalogSensor& AnalogSensor::setInterval(unsigned long interval) {
    _interval = interval;
    return *this;
}

AnalogSensor& AnalogSensor::setCallback(void (*callback)(int)) {
    _callback = callback;
    return *this;
}

int AnalogSensor::read() {
    _lastReadValue = analogRead(_pin);
    return _lastReadValue; // Return the value immediately after reading
}

void AnalogSensor::loop() {
    unsigned long currentTime = millis();
    if (currentTime - _lastReadTime >= _interval) {
        int value = read();
        if (_callback) _callback(value);
        _lastReadTime = currentTime;
    }
}
