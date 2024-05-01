#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Arduino.h"
#include <EEPROM.h>

#define CAL_LOW_ADDR 0
#define CAL_HIGH_ADDR sizeof(int)
#define INTERVAL_ADDR (CAL_HIGH_ADDR + sizeof(int))

class AnalogSensor {
public:
    AnalogSensor(int pin);
    AnalogSensor& setInterval(unsigned long interval);
    AnalogSensor& setCallback(void (*callback)(int));
    AnalogSensor& setCalibrationLow(int low);
    AnalogSensor& setCalibrationHigh(int high);
    AnalogSensor& loadCalibration();
    AnalogSensor& loadInterval();
    int read();
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
