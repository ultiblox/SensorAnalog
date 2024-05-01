#include "AnalogSensor.h"

void handleAnalogSensorData(int value) {
    Serial.println(value);
}

AnalogSensor analogSensor(A0);

void setup() {
    Serial.begin(115200);
    analogSensor.setInterval(5000)
      .setCallback(handleAnalogSensorData);
}

void loop() {
    // Handle sensor via loop
    analogSensor.loop();

    // Read sensor directly
    // int value = analogSensor.read();
    // Serial.println(value);

    delay(100); // This delay helps prevent flooding the loop; adjust as needed
}
