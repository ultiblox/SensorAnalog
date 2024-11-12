#include "SensorAnalog.h"
#include "LoggerConfig.h"
#include "SerialLogger.h"

SensorAnalog sensor(A0);

SerialLogger Logger;

void setup() {
    Logger.begin(115200);
    Logger.println("Serial started...");
    
    sensor.init()
          .setCalibrationLow(0)
          .setCalibrationHigh(1024)
          .setSampleSize(10);
}

void loop() {
    int rawValue = sensor.readRaw();
    
    int calibratedValue = sensor.read();

    Logger.data("Raw", "R", rawValue);
    Logger.data("Calibrated", "C", calibratedValue);
    Logger.dataln();

    delay(100);
}

