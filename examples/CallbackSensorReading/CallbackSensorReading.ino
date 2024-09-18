#include <SensorAnalog.h>

SensorAnalog sensor(A0);  // Use pin A0 for the sensor

// Callback function to process sensor data
void sensorDataCallback(int calibratedValue) {
    Serial.print("Calibrated sensor value: ");
    Serial.println(calibratedValue);
}

void setup() {
    Serial.begin(9600);
    
    // Set calibration range and interval, and provide callback
    sensor.setCalibrationLow(100)
          .setCalibrationHigh(900)
          .setInterval(1000)
          .setCallback(sensorDataCallback);
}

void loop() {
    // Update the sensor data at the specified interval
    sensor.loop();
}
