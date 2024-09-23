#include "SensorAnalog.h"
#include "SerialLogger.h"  // Logger for debugging, enabled/disabled by compiler conditions

SensorAnalog::SensorAnalog(int pin)
    : _pin(pin), _interval(1000), _lastReadTime(0), _callback(nullptr), _calLow(100), _calHigh(900) {
    pinMode(_pin, INPUT);

    Logger.logln("Constructing SensorAnalog...");
    // Set default calibration values in case no EEPROM data is available
    setCalibrationDefaultLow(100);
    setCalibrationDefaultHigh(900);

    // Attempt to load calibration from EEPROM (will override defaults if found)
    loadCalibration();

    // Load reading interval from EEPROM
    loadInterval();

    //LOG_INFO("SensorAnalog initialized on pin " + String(_pin));
}

// Sets default low calibration value without storing it in EEPROM
SensorAnalog& SensorAnalog::setCalibrationDefaultLow(int low) {
    _calLow = low;
    //LOG_INFO("Default calibration low set to " + String(_calLow));
    return *this;
}

// Sets default high calibration value without storing it in EEPROM
SensorAnalog& SensorAnalog::setCalibrationDefaultHigh(int high) {
    _calHigh = high;
    //LOG_INFO("Default calibration high set to " + String(_calHigh));
    return *this;
}

// Sets the reading interval and saves it in EEPROM
SensorAnalog& SensorAnalog::setInterval(unsigned long interval) {
    _interval = interval;
    EEPROM.put(INTERVAL_ADDR, _interval);
    //LOG_INFO("Sensor read interval set to " + String(_interval) + " ms");
    return *this;
}

// Sets the callback function for sensor readings
SensorAnalog& SensorAnalog::setCallback(void (*callback)(int)) {
    _callback = callback;
    //LOG_INFO("Callback set for sensor readings.");
    return *this;
}

// Stores user-set low calibration value in EEPROM
SensorAnalog& SensorAnalog::setCalibrationLow(int low) {
    _calLow = low;
    EEPROM.put(CAL_LOW_ADDR, _calLow);
    EEPROM.put(CAL_LOW_FLAG_ADDR, VALID_FLAG);
    //Logger.log("Setting sensor low: ");
    //Logger.logln(_calLow);
    //LOG_INFO("Calibration low saved to EEPROM: " + String(_calLow));
    return *this;
}

// Stores user-set high calibration value in EEPROM
SensorAnalog& SensorAnalog::setCalibrationHigh(int high) {
    _calHigh = high;
    EEPROM.put(CAL_HIGH_ADDR, _calHigh);
    EEPROM.put(CAL_HIGH_FLAG_ADDR, VALID_FLAG);
    //LOG_INFO("Calibration high saved to EEPROM: " + String(_calHigh));
    //Logger.log("Setting sensor high: ");
    //Logger.logln(_calHigh);
    //Serial.println("Calibration high saved to EEPROM: " + String(_calHigh));
    return *this;
}

// Loads calibration values from EEPROM if available; uses defaults if not found
SensorAnalog& SensorAnalog::loadCalibration() {
    int flagLow, flagHigh;
    EEPROM.get(CAL_LOW_FLAG_ADDR, flagLow);
    EEPROM.get(CAL_HIGH_FLAG_ADDR, flagHigh);
    
    //Logger.logln("Loading calibration...");
    //LOG_INFO("Loading calibration");
    //Serial.println("Loading calibration values from EEPROM");

    // Load calibration from EEPROM only if valid flags are set
    if (flagLow == VALID_FLAG) {
        EEPROM.get(CAL_LOW_ADDR, _calLow);
        Logger.log("Sensor low: ");
        Logger.logln(_calLow);
    } else {
        // LOG_DEBUG("Using default low calibration: %d", _calLow);
    }

    if (flagHigh == VALID_FLAG) {
        EEPROM.get(CAL_HIGH_ADDR, _calHigh);
        
    //Serial.print("high ");
    //Serial.print(_calHigh);
    //Serial.println();
        // LOG_DEBUG("Loaded high calibration from EEPROM: %d", _calHigh);

        Logger.log("Sensor high: ");
        Logger.logln(_calHigh);
    } else {
        // LOG_DEBUG("Using default high calibration: %d", _calHigh);
    }

    return *this;
}

// Loads the interval time for sensor readings from EEPROM, defaults to 1000ms if unset
SensorAnalog& SensorAnalog::loadInterval() {
    EEPROM.get(INTERVAL_ADDR, _interval);
    if (_interval == 0xFFFFFFFF) {
        _interval = 1000;
    }
    //LOG_INFO("Sensor interval loaded: %d ms", _interval);
    //LOG_INFO("Sensor interval loaded: " << _interval);

    return *this;
}

// Reads raw sensor value from the analog pin
int SensorAnalog::readRaw() {
    _lastReadValue = analogRead(_pin);
    //LOG_INFO("Raw sensor value: %d", _lastReadValue);
    //LOG_INFO("Raw sensor value: " << _lastReadValue);
    return _lastReadValue;
}

// Reads calibrated sensor value, scaled between 0 and 100
int SensorAnalog::readCalibrated() {
    int calibratedValue = mapSensorValue(_lastReadValue, _calLow, _calHigh, 0, 100);
    //LOG_INFO("Calibrated sensor value: %d", calibratedValue);
    //LOG_INFO("Calibrated sensor value: " << calibratedValue);
    return calibratedValue;
}

// Returns the current low calibration value
int SensorAnalog::readCalibrationLow() {
    return _calLow;
}

// Returns the current high calibration value
int SensorAnalog::readCalibrationHigh() {
    return _calHigh;
}

// Main loop: periodically reads sensor data and invokes the callback function
void SensorAnalog::loop() {
    unsigned long currentTime = millis();
    if (currentTime - _lastReadTime >= _interval) {
        int value = readCalibrated();  // Read the calibrated value
        if (_callback) _callback(value);
        //LOG_DEBUG("Sensor loop triggered. Calibrated value:  %d", value);
        //LOG_DEBUG("Sensor loop triggered. Calibrated value: " << value);
        _lastReadTime = currentTime;
    }
}

// Maps the sensor value from the calibration range to a 0-100% scale
int SensorAnalog::mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
    // Ensure valid input range (invert ranges if needed)
    if (fromLow > fromHigh) {
        int temp = fromLow;
        fromLow = fromHigh;
        fromHigh = temp;

        temp = toLow;
        toLow = toHigh;
        toHigh = temp;
    }

    // Constrain value to the specified range
    value = constrain(value, fromLow, fromHigh);

    // Map the value to the output range
    float scale = float(toHigh - toLow) / float(fromHigh - fromLow);
    int mappedValue = (value - fromLow) * scale + toLow;

    // Constrain the mapped value to the output range
    return constrain(mappedValue, min(toLow, toHigh), max(toLow, toHigh));
}
