#include "SensorAnalog.h"
#include "LoggerConfig.h"  // Use updated LoggerConfig
#include "SerialLogger.h"

SensorAnalog sensor(A0);  // Use pin A0 for the sensor

SerialLogger Logger;

void setup() {
    Logger.begin(115200);
    Logger.println("Serial started...");
    
    // Set up the sensor
    sensor.init()
          .setCalibrationLow(100)
          .setCalibrationHigh(900);
}

void loop() {
    // Read the raw value
    int rawValue = sensor.readRaw();
    
    // Read the calibrated value
    int calibratedValue = sensor.read();

    Logger.data("Raw", "R", rawValue);
    Logger.data("Calibrated", "C", calibratedValue);
    Logger.dataln();

    // Wait before reading again
    delay(1000);
}

