#include "SensorAnalog.h"
#include "LoggerConfig.h"
#include "SerialLogger.h"

SerialLogger Logger;
SensorAnalog sensor(A0);  // Use pin A0 for the sensor

void setup() {
    Logger.begin(115200);
    Logger.println("Serial started...");
    
    // Set up the sensor
    sensor.init()
          .setCalibrationLow(100)
          .setCalibrationHigh(900)
          .setInterval(1000)
          .onDataReceived(handleDataReceived);
}

void loop() {
    // Trigger a sensor processing loop
    sensor.loop();
}

// Event handler for data received
void handleDataReceived(int calibratedValue) {  
    int rawValue = sensor.readRaw();
    
    Logger.data("Calibrated", "C", calibratedValue);    
    Logger.data("Raw", "R", rawValue);
    Logger.dataln();
}

