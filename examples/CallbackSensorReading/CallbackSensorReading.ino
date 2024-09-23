#include <SensorAnalog.h>
#include "LoggerConfig.h"  // Include logger configuration
#include "SerialLogger.h"

SensorAnalog sensor(A0);  // Use pin A0 for the sensor

// Callback function to process sensor data
void sensorDataCallback(int calibratedValue) {
    int rawValue = sensor.readRaw();
    Serial.println("Raw: " + String(rawValue) + ", Calibrated: " + String(calibratedValue));  // Log sensor readings
}

void setup() {
    Serial.begin(115200);
    Serial.println("Serial started...");  // Fallback for serial output
    //LOG_INFO("Setting up sensor...");
    
    // Set calibration range and interval, and provide callback
    sensor.setCalibrationDefaultLow(100)
          .setCalibrationDefaultHigh(900)
          .setInterval(1000)
          .loadCalibration()
          .setCallback(sensorDataCallback);

    //LOG_INFO("Sensor setup complete.");
}

void loop() {
    // Update the sensor data at the specified interval
    sensor.loop();
}

