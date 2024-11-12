#include "SensorAnalog.h"
#include "LoggerConfig.h"
#include "SerialLogger.h"

SerialLogger Logger;
SensorAnalog sensor(A0);

void setup() {
    Logger.begin(115200);
    Logger.println("Serial started...");
    
    sensor.init()
          .setCalibrationLow(0)
          .setCalibrationHigh(1024)
          .setInterval(100)
          .setSampleSize(10)
          .onDataReceived(handleDataReceived);
}

void loop() {
    sensor.loop();
}

void handleDataReceived(int calibratedValue) {  
    int rawValue = sensor.readRaw();
    
    Logger.data("Calibrated", "C", calibratedValue);    
    Logger.data("Raw", "R", rawValue);
    Logger.dataln();
}

