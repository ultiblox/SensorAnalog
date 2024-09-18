#include "SensorAnalog.h"

SensorAnalog analogSensor(A0); // Create an instance of the AnalogSensor on pin A0

void handleSensorData(int value) {
  int rawValue = analogSensor.readRaw();
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(", Calibrated Value: ");
  Serial.println(value);
}

void setup() {
  Serial.begin(115200);
  
  // Set the interval at which sensor readings and calibration are processed
  analogSensor.setInterval(1000)
      .setDefaultCalibrationHigh(900)
      .setDefaultCalibrationLow(100)
      .setCallback(handleSensorData)
      .loadCalibration();

  Serial.println("Sensor initialized and calibrated. Reading values...");
}

void loop() {
  // Handle sensor via loop
  analogSensor.loop();

  delay(1000);
}
