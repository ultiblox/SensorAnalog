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
private:
    int _pin;
    unsigned long _interval;
    unsigned long _lastReadTime;
    int _lastReadValue;
    int _calLow;
    int _calHigh;
    void (*_callback)(int);
    
    int mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh);

public:
    SensorAnalog(int pin);

    SensorAnalog& init();
    int read();  // Calibrated sensor reading
    int readRaw();  // Raw sensor reading
    SensorAnalog& setInterval(unsigned long interval);
    SensorAnalog& onDataReceived(void (*callback)(int));
    void loop();

    SensorAnalog& setCalibrationLow(int low);
    SensorAnalog& setCalibrationHigh(int high);
    SensorAnalog& loadCalibration();
    SensorAnalog& setCalibrationDefaultLow(int low);
    SensorAnalog& setCalibrationDefaultHigh(int high);

    int readCalibrationLow();
    int readCalibrationHigh();
    SensorAnalog& loadInterval();
};

#endif

