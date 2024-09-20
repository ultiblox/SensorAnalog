#include "SensorAnalog.h"
#include "SerialLogger.h"  // Use SerialLogger

SensorAnalog::SensorAnalog(int pin)
    : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr), _calLow(0), _calHigh(1023) {
    pinMode(_pin, INPUT);
    loadCalibration();  // Load calibration values from EEPROM
    loadInterval();     // Load interval from EEPROM

    LOG_INFO("SensorAnalog initialized on pin " + String(_pin));
}

SensorAnalog& SensorAnalog::setInterval(unsigned long interval) {
    _interval = interval;
    EEPROM.put(INTERVAL_ADDR, _interval);
    LOG_INFO("Sensor read interval set to " + String(_interval) + " ms");

    return *this;
}

SensorAnalog& SensorAnalog::setCallback(void (*callback)(int)) {
    _callback = callback;
    LOG_INFO("Callback set for sensor readings.");
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationLow(int low) {
    _calLow = low;
    EEPROM.put(CAL_LOW_ADDR, _calLow);
    EEPROM.put(CAL_LOW_FLAG_ADDR, 1);
    LOG_INFO("Calibration low set to " + String(_calLow));
    return *this;
}

SensorAnalog& SensorAnalog::setCalibrationHigh(int high) {
    _calHigh = high;
    EEPROM.put(CAL_HIGH_ADDR, _calHigh);
    EEPROM.put(CAL_HIGH_FLAG_ADDR, 1);
    LOG_INFO("Calibration high set to " + String(_calHigh));
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
    LOG_INFO("Calibration loaded: Low = " + String(_calLow) + ", High = " + String(_calHigh));

    return *this;
}

SensorAnalog& SensorAnalog::loadInterval() {
    EEPROM.get(INTERVAL_ADDR, _interval);
    if (_interval == 0xFFFFFFFF) {
        _interval = 1000;
    }
    LOG_INFO("Sensor interval loaded: " + String(_interval) + " ms");

    return *this;
}

int SensorAnalog::readRaw() {
    _lastReadValue = analogRead(_pin);
    LOG_INFO("Raw sensor value: " + String(_lastReadValue));
    return _lastReadValue;
}

int SensorAnalog::readCalibrated() {
    int calibratedValue = mapSensorValue(_lastReadValue, _calLow, _calHigh, 0, 100);
    LOG_INFO("Calibrated sensor value: " + String(calibratedValue));
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
        int value = readCalibrated();
        if (_callback) _callback(value);
        LOG_INFO("Sensor loop triggered. Calibrated value: " + String(value));
        _lastReadTime = currentTime;
    }
}

int SensorAnalog::mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    // Detect if the input range is inverted and manually swap if needed
    if (fromLow > fromHigh) {
        int temp = fromLow;
        fromLow = fromHigh;
        fromHigh = temp;

        temp = toLow;
        toLow = toHigh;
        toHigh = temp;
    }

    // Constrain the input value to the input range
    value = constrain(value, fromLow, fromHigh);

    // Calculate the scale for the mapping
    float scale = float(toHigh - toLow) / float(fromHigh - fromLow);

    // Map the constrained value
    int mappedValue = (value - fromLow) * scale + toLow;

    // Constrain the output value to the output range using Arduino's min/max
    return constrain(mappedValue, min(toLow, toHigh), max(toLow, toHigh));
}


