#ifndef SensorAnalog_h
#define SensorAnalog_h

#include "Arduino.h"
#include <EEPROM.h>

#define VALID_FLAG 0xCAFE

#define CAL_LOW_ADDR 0
#define CAL_HIGH_ADDR (CAL_LOW_ADDR + sizeof(int))
#define CAL_LOW_FLAG_ADDR (CAL_HIGH_ADDR + sizeof(int))
#define CAL_HIGH_FLAG_ADDR (CAL_LOW_FLAG_ADDR + sizeof(int))
#define INTERVAL_ADDR (CAL_HIGH_FLAG_ADDR + sizeof(int))

class SensorAnalog {
public:
    SensorAnalog(int pin);
    SensorAnalog& setInterval(unsigned long interval);
    SensorAnalog& onDataReceived(void (*callback)(int));
    SensorAnalog& setCalibrationLow(int low);
    SensorAnalog& setCalibrationHigh(int high);
    SensorAnalog& setCalibrationDefaultLow(int low);
    SensorAnalog& setCalibrationDefaultHigh(int high); // New method to set the suffix
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
    char suffix[8] = "";  // Storage for the suffix
    int mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh);
};

#endif