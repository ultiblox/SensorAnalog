#include <SensorAnalog.h>
#include "LoggerConfig.h"  // Use updated LoggerConfig
#include "SerialLogger.h"

SensorAnalog sensor(A0);  // Use pin A0 for the sensor

void setup() {
    Serial.begin(115200);
    Serial.println("Serial started...");
    LOG_INFO("Setting up sensor...");

    // Set calibration range
    sensor.setCalibrationLow(100)
          .setCalibrationHigh(900);

    LOG_INFO("Sensor setup complete.");
}

void loop() {
    // Read the raw value
    int rawValue = sensor.readRaw();
    //LOG_INFO("Raw sensor value: " + String(rawValue));

    // Read the calibrated value
    int calibratedValue = sensor.readCalibrated();
    //LOG_INFO("Calibrated sensor value: " + String(calibratedValue));

    // Wait before reading again
    delay(1000);
}
