#include "AnalogSensor.h"

AnalogSensor analogSensor(A0); // Create an instance of the AnalogSensor on pin A0

void handleSensorData(int value) {
  int calibratedValue = analogSensor.readCalibrated();
  Serial.print("Raw Value: ");
  Serial.print(value);
  Serial.print(", Calibrated Value: ");
  Serial.println(calibratedValue);
}

void setup() {
  Serial.begin(115200);
  
  // Set the interval at which sensor readings and calibration are processed
  analogSensor.setInterval(1000); // Set to 5000 milliseconds (5 seconds)
  
  // Set initial calibration values
  analogSensor.setCalibrationLow(100);  // Example low calibration value
  analogSensor.setCalibrationHigh(900); // Example high calibration value
  
  // Optionally load calibration from EEPROM if it's already set previously
  analogSensor.loadCalibration();

  analogSensor.setCallback(handleSensorData); // Assign the callback function

  Serial.println("Sensor initialized and calibrated. Reading values...");
}

void loop() {
  // Handle sensor via loop
  analogSensor.loop();

  delay(1000);
}
