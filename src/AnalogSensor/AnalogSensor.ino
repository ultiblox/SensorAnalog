#include "AnalogSensor.h"

AnalogSensor sensor(A0); // Create an instance of AnalogSensor on analog pin A0

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = sensor.read(); // Read the analog value from sensor
  Serial.println(sensorValue);
  delay(1000); // Delay for a second for readability of output
}
