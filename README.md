# SensorAnalog Library

## Overview
The `SensorAnalog` library provides functionality to read and calibrate analog sensors connected to Arduino boards. It supports reading raw sensor data, applying custom calibration ranges, and triggering callbacks at specified intervals.

## Features
- Customizable calibration range for sensor values.
- Periodic sensor reading at user-defined intervals.
- Callback mechanism to process sensor data.
- EEPROM-based storage for calibration data.

## Usage

### Initialization
```cpp
#include "SensorAnalog.h"
SensorAnalog SensorAnalog(A0);
```

### Set Calibration
```cpp
SensorAnalog.setCalibrationLow(100)
          .setCalibrationHigh(900);
```

### Set Interval and Callback
```cpp
SensorAnalog.setInterval(1000)
          .setCallback(handleSensorData);
```

## Examples

### 1. Callback Sensor Reading
This example demonstrates how to use the callback mechanism to automatically read sensor values at a specified interval.
- Example: `examples/CallbackSensorReading/CallbackSensorReading.ino`

### 2. Manual Sensor Reading
This example demonstrates how to manually read both raw and calibrated sensor values.
- Example: `examples/ManualSensorReading/ManualSensorReading.ino`

## Installation
You can install the **SensorAnalog** library in two ways: by copying it to your Arduino libraries folder or by creating a symlink for easier development.

### 1. Standard Installation (Copy)
To install the library by copying it to the Arduino libraries folder, run the following command:
```bash
bash install.sh
```
This will copy the library to the Arduino libraries folder at `~/Arduino/libraries/SensorAnalog`.

### 2. Development Installation (Symlink)
If you are actively developing the library and want to create a symbolic link instead of copying the files, run:
```bash
bash install-symlink.sh
```
This will create a symlink from your current workspace to the Arduino libraries folder, so any changes you make in the workspace will reflect immediately in the Arduino IDE.

## Building the Library
You can compile the example sketches using the **`build.sh`** script:
```bash
bash build.sh
```
This script uses the Arduino CLI to compile all the example sketches in the library.

## Preparing the Environment
If you don't have Arduino CLI installed, you can install it using the **`prepare.sh`** script:
```bash
bash prepare.sh
```
This script will install the latest version of Arduino CLI to your system or user directory.

## Dependencies
- Arduino Core Library
- EEPROM Library

## License
This library is open source, distributed under the MIT License.
