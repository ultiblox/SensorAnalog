#include "SensorAnalog.h"

SensorAnalog::SensorAnalog(int pin)
    : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr),
      _calLow(100), _calHigh(900), _average(0), _sampleCount(0), _lastSampleTime(0) {
    pinMode(_pin, INPUT);
    setCalibrationDefaultLow(100);
    setCalibrationDefaultHigh(900);
    loadCalibration();
    loadInterval();
}

SensorAnalog& SensorAnalog::init() {
    pinMode(_pin, INPUT);
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationDefaultLow(int low) {
    _calLow = low;
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationDefaultHigh(int high) {
    _calHigh = high;
    return *this;
}

SensorAnalog& SensorAnalog::setInterval(unsigned long interval) {
    _interval = interval;
    EEPROM.put(INTERVAL_ADDR, _interval);
    return *this;
}

SensorAnalog& SensorAnalog::onDataReceived(void (*callback)(int)) {
    _callback = callback;
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationLow(int low) {
    _calLow = low;
    EEPROM.put(CAL_LOW_ADDR, _calLow);
    EEPROM.put(CAL_LOW_FLAG_ADDR, VALID_FLAG);
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationHigh(int high) {
    _calHigh = high;
    EEPROM.put(CAL_HIGH_ADDR, _calHigh);
    EEPROM.put(CAL_HIGH_FLAG_ADDR, VALID_FLAG);
    return *this;
}

SensorAnalog& SensorAnalog::loadCalibration() {
    int flagLow, flagHigh;
    EEPROM.get(CAL_LOW_FLAG_ADDR, flagLow);
    EEPROM.get(CAL_HIGH_FLAG_ADDR, flagHigh);
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
    if (_interval == 0xFFFFFFFF) _interval = 1000;
    return *this;
}

SensorAnalog& SensorAnalog::setSampleSize(int size) {
    if (size <= 0) size = 1;  // Minimum size is 1
    _sampleCount = 0;         // Reset sample count
    _average = 0;             // Reset average
    return *this;
}

void SensorAnalog::addSample(int value) {
    _sampleCount++;
    _average += (value - _average) / _sampleCount;  // Incremental CMA formula
}

int SensorAnalog::calculateAverage() const {
    return static_cast<int>(_average);  // Return rounded average
}

int SensorAnalog::readRaw() {
    _lastReadValue = analogRead(_pin);
    return _lastReadValue;
}

int SensorAnalog::read() {
    addSample(readRaw());  // Initialize `_average` with the first raw sample
    return mapSensorValue(static_cast<int>(_average), _calLow, _calHigh, 0, 100);
}

void SensorAnalog::loop() {
    // Ensure enough time has elapsed for the callback interval
    if (millis() - _lastReadTime >= _interval) {
        if (_callback) _callback(read());  // Pass mapped value to the callback
        _lastReadTime = millis();
    }
}

int SensorAnalog::mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    // Constrain the raw value within the calibration range (handles both normal and inverted ranges)
    value = constrain(value, min(fromLow, fromHigh), max(fromLow, fromHigh));

    // Use Arduino's built-in map() function, which can handle inverted ranges
    return map(value, fromLow, fromHigh, toLow, toHigh);
}