# UltiBlox SensorAnalog Library
*Easily read and calibrate analog sensor values with custom intervals and callbacks.*

[UltiBlox on GitHub](https://github.com/UltiBlox/SensorAnalog) | [UltiBlox Home](https://ultiblox.org)

## Overview
The `SensorAnalog` library enables straightforward reading and calibration of analog sensors connected to Arduino boards. It includes customizable calibration ranges, periodic reading with user-defined intervals, and callbacks for handling data, with optional storage of calibration data in EEPROM for persistence.

## Features
- **Custom Calibration Range**: Define low and high calibration values to map sensor readings.
- **Periodic Reading with Callbacks**: Set an interval for automated sensor polling, with callback support.
- **EEPROM Storage**: Optionally store calibration values and intervals in EEPROM.
- **Flexible Data Access**: Read raw or calibrated sensor values as needed.

## Installation

### Option 1: Arduino Library Manager Installation (Recommended)
1. Open the **Arduino IDE**.
2. Go to **Tools > Manage Libraries**.
3. Search for **UltiBlox SensorAnalog** and click **Install**.
4. Access example sketches under **File > Examples > UltiBlox SensorAnalog**.

### Option 2: Manual Installation (for Development and Customization)
1. **Clone the Repository**:
   ```bash
   git clone git@github.com:UltiBlox/SensorAnalog.git ~/workspace/SensorAnalog
   cd ~/workspace/SensorAnalog
   ```

2. **Prepare the Environment**:
   Run the `prepare.sh` script to set up dependencies:
   ```bash
   bash prepare.sh
   ```

3. **Install the Library**:
   - **Copy Installation**:
     ```bash
     bash install.sh
     ```
   - **Symlink Installation** (for active development):
     ```bash
     bash install-symlink.sh
     ```

4. **Build Examples**:
   Compile example sketches with:
   ```bash
   bash build.sh
   ```

## Dependencies
- **EEPROM Library**: Required for storing calibration data.
- **Arduino Core Library**
- **SerialLogger**: Enables serial output for logging sensor values.

## Examples Directory
- **[CallbackSensorReading.ino](examples/CallbackSensorReading/CallbackSensorReading.ino)**: Sets up periodic sensor reading with a callback function to log calibrated and raw values.
- **[ManualSensorReading.ino](examples/ManualSensorReading/ManualSensorReading.ino)**: Manually reads and logs both raw and calibrated sensor values.

## Methods

### Initialize the Sensor
```cpp
init()
```
Initializes the sensor, including setting the pin mode and loading any stored calibration values.

### Set Calibration Defaults
```cpp
setCalibrationDefaultLow(int low)
setCalibrationDefaultHigh(int high)
```
Sets default calibration values for the sensor. These values are loaded at startup if EEPROM does not contain stored calibration values.

### Set Calibration Range
```cpp
setCalibrationLow(int low)
setCalibrationHigh(int high)
```
Defines the low and high values for calibration mapping, saving these values to EEPROM for persistence.

### Load Calibration Data
```cpp
loadCalibration()
```
Loads calibration values from EEPROM if available.

### Set Interval and Callback
```cpp
setInterval(unsigned long interval)
onDataReceived(void (*callback)(int))
```
Sets the interval for periodic sensor reads (in milliseconds) and specifies a callback function that executes on each read.

### Read Sensor Values
```cpp
readRaw()
```
Returns the raw sensor reading directly from the analog pin.

```cpp
read()
```
Returns the calibrated sensor reading, mapped to a scale from 0 to 100.

## License
[This project is licensed under the UltiBlox License.](https://ultiblox.org/license)
