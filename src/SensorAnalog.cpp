#include "SensorAnalog.h"

SensorAnalog::SensorAnalog(int pin) : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr), _calLow(0), _calHigh(1023) {
    pinMode(_pin, INPUT);
    loadCalibration(); // Load calibration values from EEPROM when initializing
    loadInterval();    // Load interval from EEPROM
}

SensorAnalog& SensorAnalog::setInterval(unsigned long interval) {
    _interval = interval;
    EEPROM.put(INTERVAL_ADDR, _interval);
    return *this;
}

SensorAnalog& SensorAnalog::setCallback(void (*callback)(int)) {
    _callback = callback;
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationLow(int low) {
    _calLow = low;
    EEPROM.put(CAL_LOW_ADDR, _calLow);
    EEPROM.put(CAL_LOW_FLAG_ADDR, 1);  // Set the flag to indicate EEPROM has customized value
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationHigh(int high) {
    _calHigh = high;
    EEPROM.put(CAL_HIGH_ADDR, _calHigh);
    EEPROM.put(CAL_HIGH_FLAG_ADDR, 1);  // Set the flag to indicate EEPROM has customized value
    return *this;
}

// Set default calibration values without affecting EEPROM
SensorAnalog& SensorAnalog::setDefaultCalibrationLow(int low) {
    _calLow = low;
    return *this;
}

SensorAnalog& SensorAnalog::setDefaultCalibrationHigh(int high) {
    _calHigh = high;
    return *this;
}

SensorAnalog& SensorAnalog::loadCalibration() {
    int flagLow, flagHigh;

    EEPROM.get(CAL_LOW_FLAG_ADDR, flagLow);
    EEPROM.get(CAL_HIGH_FLAG_ADDR, flagHigh);

    // Load calibration only if flags indicate that values have been stored
    if (flagLow == VALID_FLAG) {
        EEPROM.get(CAL_LOW_ADDR, _calLow);
    }
    if (flagHigh == VALID_FLAG) {
        EEPROM.get(CAL_HIGH_ADDR, _calHigh);
    }
    return *this;
}

SensorAnalog& SensorAnalog::loadInterval() {
    EEPROM.get(INTERVAL_ADDR, _interval);
    if (_interval == 0xFFFFFFFF) {  // EEPROM was never set, use default
        _interval = 1000;  // Default interval of 1000 milliseconds
    }
    return *this;
}

int SensorAnalog::readRaw() {
    _lastReadValue = analogRead(_pin);
    return _lastReadValue;
}

int SensorAnalog::readCalibrated() {
    int calibratedValue = mapSensorValue(_lastReadValue, _calLow, _calHigh, 0, 100);
    return calibratedValue;
}

int SensorAnalog::readCalibrationLow() {
    return _calLow;
}

int SensorAnalog::readCalibrationHigh() {
    return _calHigh;
}

void SensorAnalog::loop() {
    unsigned long currentTime = millis();
    if (currentTime - _lastReadTime >= _interval) {
        int value = readCalibrated(); // Store the last read value
        if (_callback) _callback(value);
        _lastReadTime = currentTime;
    }
}

int SensorAnalog::mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    // First, constrain the value to the expected range of the sensor to avoid extrapolation.
    value = constrain(value, fromLow, fromHigh);

    // Compute the ratio as a float to avoid truncation errors during division.
    float scale = float(toHigh - toLow) / float(fromHigh - fromLow);
    int mappedValue = (value - fromLow) * scale + toLow;

    // Further constrain the output to ensure it does not go out of bounds due to rounding errors.
    return constrain(mappedValue, toLow, toHigh);
}


