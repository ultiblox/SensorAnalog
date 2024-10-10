# SensorAnalog Library

**SensorAnalog** is an Arduino library that simplifies reading and calibrating analog sensors. It provides easy access to both raw and calibrated sensor data, supports periodic reading with callback mechanisms, and includes EEPROM-based calibration storage.

## Features
- Custom calibration ranges for sensor values.
- Periodic sensor reading with callback-based event handling.
- Manual sensor reading for more control over when values are fetched.
- EEPROM-based storage for saving and restoring calibration data across power cycles.

---

## Installation

There are several ways to install the **SensorAnalog** library, depending on whether you're using the library as-is or planning to develop and customize it.

### Option 1: Simple Installation (For General Use)

If you're using the library without modifications, the easiest way to install it is by cloning it directly into your Arduino libraries folder.

```bash
git clone https://github.com/makers-multiverse/SensorAnalog.git ~/Arduino/libraries/SensorAnalog
cd ~/Arduino/libraries/SensorAnalog
bash prepare.sh
```

This will set up the library and install any additional required dependencies.

### Option 2: Symlink Installation (For Active Development)

If you plan to actively develop or modify the library, you should clone the project into a workspace and create a symlink to the Arduino libraries folder. This allows you to edit the library code and see changes reflected instantly in the Arduino IDE.

```bash
git clone https://github.com/makers-multiverse/SensorAnalog.git ~/workspace/SensorAnalog
cd ~/workspace/SensorAnalog
bash install-symlink.sh
```

This method makes development easier by allowing real-time changes to reflect in the IDE.

### Option 3: Standard Installation (For Customization Without Active Development)

If you want to customize the library but don’t need real-time updates, you can copy it into your Arduino libraries folder from your workspace.

```bash
git clone https://github.com/makers-multiverse/SensorAnalog.git ~/workspace/SensorAnalog
cd ~/workspace/SensorAnalog
bash install.sh
```

This will copy the library from your workspace into your Arduino libraries folder.

---

## Dependencies

Before using the **SensorAnalog** library, ensure the following Arduino libraries are installed:

- **EEPROM Library** (used for storing calibration data).
- **SerialLogger** (used for logging sensor data).

If these libraries aren’t already installed, the **prepare.sh** script will handle the installation for you. Alternatively, you can install them manually via the Arduino Library Manager.

---

## Usage

The **SensorAnalog** library can be used in two primary ways: **manual sensor reading** or **callback-based sensor reading**. Below are examples of both approaches.

### 1. Manual Sensor Reading

This method allows you to manually control when sensor values are read, and you can retrieve both raw and calibrated data.

```cpp
#include "SensorAnalog.h"

SensorAnalog sensor(A0);  // Use analog pin A0 for sensor readings

void setup() {
    // Initialize the sensor and set calibration ranges
    sensor.init()
          .setCalibrationLow(100)   // Set the minimum calibration value (raw input 100)
          .setCalibrationHigh(900); // Set the maximum calibration value (raw input 900)
}

void loop() {
    int rawValue = sensor.readRaw();         // Get the raw sensor value
    int calibratedValue = sensor.read();     // Get the calibrated sensor value

    // Do something with the sensor values
    delay(1000);  // Wait 1 second before reading again
}
```

In this example:
- `sensor.readRaw()` retrieves the uncalibrated raw value from the sensor.
- `sensor.read()` retrieves the calibrated value, which maps the raw value to a user-defined range (e.g., temperature in Celsius).

### 2. Callback-Based Sensor Reading (Periodic Reading)

The callback-based approach allows the sensor to automatically read values at regular intervals and process them using a callback function. This is useful for event-driven applications where you need periodic data collection without manually controlling the loop.

```cpp
#include "SensorAnalog.h"
#include "SerialLogger.h"

SerialLogger Logger;   // Logger for printing data to serial monitor
SensorAnalog sensor(A0);  // Use analog pin A0 for sensor readings

void setup() {
    Logger.begin(115200);
    Logger.println("Serial started...");

    // Initialize the sensor, set calibration, and specify a 1-second interval for reading
    sensor.init()
          .setCalibrationLow(100)
          .setCalibrationHigh(900)
          .setInterval(1000)               // Set sensor to read every 1000 ms (1 second)
          .onDataReceived(handleData);     // Set callback function to handle sensor data
}

void loop() {
    sensor.loop();  // Required to process sensor readings at set intervals
}

// Callback function to handle sensor data
void handleData(int calibratedValue) {
    int rawValue = sensor.readRaw();      // Read the raw value as well
    Logger.data("Calibrated", "C", calibratedValue);
    Logger.data("Raw", "R", rawValue);
    Logger.dataln();  // Print the values
}
```

In this example:
- The `sensor.setInterval(1000)` method sets the sensor to automatically read data every 1 second.
- The `onDataReceived(handleData)` method specifies a callback function (`handleData`) that processes the sensor data each time it's read.
- `sensor.loop()` needs to be called in the main loop to ensure periodic readings occur.

---

### Sensor Calibration: Default vs. Actual Calibration

#### Default Calibration:
When you create a **SensorAnalog** object, the library uses **hardcoded default calibration values** to provide immediate functionality. These default values are:

- **Low value**: `100`
- **High value**: `900`

These values ensure that the sensor will work right away, mapping the raw sensor readings to a range of **100 to 900**. However, these defaults are **not stored** in EEPROM—they are simply the starting values used by the library unless you decide to set your own calibration.

#### Setting Actual Calibration:
If you want to map raw sensor values to a more meaningful real-world scale (e.g., temperature, pressure, etc.), you can set **custom calibration values**. These values will override the default settings and be **saved to EEPROM**, ensuring that your calibration persists across power cycles.

For example, to calibrate a temperature sensor where raw readings from **200 to 800** correspond to **0°C to 100°C**, you would set:

```cpp
sensor.setCalibrationLow(200)   // Raw value corresponding to 0°C
      .setCalibrationHigh(800); // Raw value corresponding to 100°C
```

After setting these values, any call to `sensor.read()` will map the raw sensor values into the range of **0 to 100** (or whichever real-world range you define). These custom calibration values are saved to EEPROM, meaning they are retained even after the device is powered off.

The next time your Arduino is powered on, the **loadCalibration()** function will retrieve these custom values from EEPROM. If no custom values have been stored, the default calibration will be used.

---

## Compiling and Uploading

### Using the Arduino IDE
1. Open the Arduino IDE and go to `File > Examples > SensorAnalog` to find the example sketches (e.g., `ManualSensorReading` or `CallbackSensorReading`).
2. Select the appropriate board (e.g., Arduino Nano) and COM port under `Tools > Board` and `Tools > Port`.
3. Compile and upload your sketch to your Arduino board.

### Using Arduino CLI
If you prefer the command line, you can compile and upload sketches with the Arduino CLI:

```bash
arduino-cli compile --fqbn arduino:avr:nano examples/ManualSensorReading/ManualSensorReading.ino
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano examples/ManualSensorReading/ManualSensorReading.ino
```

Be sure to replace `/dev/ttyUSB0` with the appropriate port for your board.

---

## Troubleshooting

If you encounter any issues, check the following:

1. **Libraries**: Ensure that the required libraries (EEPROM and SerialLogger) are properly installed.
2. **Board Settings**: Ensure the correct board and port are selected in the Arduino IDE.
3. **Symlink Issues (Windows)**: If using symlink installation on Windows, ensure that you are using Git Bash or another shell that supports symlinks. You may need to enable Developer Mode in Windows for symlink support.

---

## License

This library is licensed under the MIT License.