#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(int pin) : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr), _calLow(0), _calHigh(1023) {
    pinMode(_pin, INPUT);
    loadCalibration(); // Load calibration values from EEPROM when initializing
    loadInterval();    // Load interval from EEPROM
}

AnalogSensor& AnalogSensor::setInterval(unsigned long interval) {
    _interval = interval;
    EEPROM.put(INTERVAL_ADDR, _interval);
    return *this;
}

AnalogSensor& AnalogSensor::setCallback(void (*callback)(int)) {
    _callback = callback;
    return *this;
}

AnalogSensor& AnalogSensor::setCalibrationLow(int low) {
    _calLow = low;
    EEPROM.put(CAL_LOW_ADDR, _calLow);
    return *this;
}

AnalogSensor& AnalogSensor::setCalibrationHigh(int high) {
    _calHigh = high;
    EEPROM.put(CAL_HIGH_ADDR, _calHigh);
    return *this;
}

AnalogSensor& AnalogSensor::loadCalibration() {
    EEPROM.get(CAL_LOW_ADDR, _calLow);
    EEPROM.get(CAL_HIGH_ADDR, _calHigh);
    Serial.print("Loaded Calibration Low: ");
    Serial.println(_calLow);
    Serial.print("Loaded Calibration High: ");
    Serial.println(_calHigh);
    return *this;
}

AnalogSensor& AnalogSensor::loadInterval() {
    EEPROM.get(INTERVAL_ADDR, _interval);
    if (_interval == 0xFFFFFFFF) {  // EEPROM was never set, use default
        _interval = 1000;  // Default interval of 1000 milliseconds
    }
    return *this;
}

int AnalogSensor::read() {
    _lastReadValue = analogRead(_pin);
    return _lastReadValue;
}

int AnalogSensor::readCalibrated() {
    int calibratedValue = mapSensorValue(_lastReadValue, _calLow, _calHigh, 0, 100);
    return calibratedValue;
}

void AnalogSensor::loop() {
    unsigned long currentTime = millis();
    if (currentTime - _lastReadTime >= _interval) {
        int value = read(); // Store the last read value
        if (_callback) _callback(read());
        _lastReadTime = currentTime;
    }
}

int AnalogSensor::mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    // First, constrain the value to the expected range of the sensor to avoid extrapolation.
    value = constrain(value, fromLow, fromHigh);

    // Compute the ratio as a float to avoid truncation errors during division.
    float scale = float(toHigh - toLow) / float(fromHigh - fromLow);
    int mappedValue = (value - fromLow) * scale + toLow;

    // Further constrain the output to ensure it does not go out of bounds due to rounding errors.
    return constrain(mappedValue, toLow, toHigh);
}


