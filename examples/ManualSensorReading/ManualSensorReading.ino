#include <SensorAnalog.h>

SensorAnalog sensor(A0);  // Use pin A0 for the sensor

void setup() {
    Serial.begin(9600);

    // Set calibration range
    sensor.setCalibrationLow(100)
          .setCalibrationHigh(900);
}

void loop() {
    // Read the raw value
    int rawValue = sensor.readRaw();
    Serial.print("Raw sensor value: ");
    Serial.println(rawValue);

    // Read the calibrated value
    int calibratedValue = sensor.readCalibrated();
    Serial.print("Calibrated sensor value: ");
    Serial.println(calibratedValue);

    // Wait before reading again
    delay(1000);
}
