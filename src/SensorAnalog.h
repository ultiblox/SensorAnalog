#ifndef SensorAnalog_h
#define SensorAnalog_h

#include "Arduino.h"
#include <EEPROM.h>
#include "SerialLogger.h"  // Use SerialLogger instead

#define VALID_FLAG 0xCAFE

// Define calibration and interval storage locations in EEPROM
#define CAL_LOW_ADDR 0
#define CAL_HIGH_ADDR (CAL_LOW_ADDR + sizeof(int))
#define CAL_LOW_FLAG_ADDR (CAL_HIGH_ADDR + sizeof(int))
#define CAL_HIGH_FLAG_ADDR (CAL_LOW_FLAG_ADDR + sizeof(int))
#define INTERVAL_ADDR (CAL_HIGH_FLAG_ADDR + sizeof(int))

class SensorAnalog {
public:
    SensorAnalog(int pin);
    SensorAnalog& setInterval(unsigned long interval);
    SensorAnalog& setCallback(void (*callback)(int));
    SensorAnalog& setCalibrationLow(int low);
    SensorAnalog& setCalibrationHigh(int high);
    SensorAnalog& setDefaultCalibrationLow(int low);
    SensorAnalog& setDefaultCalibrationHigh(int high);
    int readCalibrationLow();
    int readCalibrationHigh();
    SensorAnalog& loadCalibration();
    SensorAnalog& loadInterval();
    int readRaw();
    int readCalibrated();
    void loop();
private:
    int _pin;
    unsigned long _interval;
    unsigned long _lastReadTime;
    int _lastReadValue;
    int _calLow;
    int _calHigh;
    void (*_callback)(int);

    int mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh);
};

#endif