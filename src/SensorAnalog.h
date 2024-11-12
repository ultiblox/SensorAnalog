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
    unsigned long _interval;       // Callback execution interval
    unsigned long _lastReadTime;   // Last callback execution time
    int _lastReadValue;            // Last raw sensor reading
    int _calLow;                   // Low calibration value
    int _calHigh;                  // High calibration value
    void (*_callback)(int);        // User-defined callback for sensor data

    float _average;                // Current cumulative moving average
    unsigned long _sampleCount;    // Total number of samples processed
    unsigned long _lastSampleTime; // Last time a sample was taken

    int mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh);
    void addSample(int value);
    int calculateAverage() const;

public:
    SensorAnalog(int pin);

    SensorAnalog& init();
    int read();  // Calibrated sensor reading (uses smoothed value)
    int readRaw();  // Raw sensor reading
    SensorAnalog& setInterval(unsigned long interval);
    SensorAnalog& onDataReceived(void (*callback)(int));
    void loop();

    SensorAnalog& setCalibrationLow(int low);
    SensorAnalog& setCalibrationHigh(int high);
    SensorAnalog& loadCalibration();
    SensorAnalog& setCalibrationDefaultLow(int low);
    SensorAnalog& setCalibrationDefaultHigh(int high);

    SensorAnalog& setSampleSize(int size);          // Adjust smoothing behavior

    SensorAnalog& loadInterval();
};

#endif
