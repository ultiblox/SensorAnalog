#!/bin/bash

# Directory for consumers
CONSUMERS_DIR="$HOME/ci-consumers"
UNIVERSAL_MAKER_BOX_REPO="https://github.com/makers-multiverse/UniversalMakerBox.git"
SENSOR_ANALOG_REPO="https://github.com/makers-multiverse/SensorAnalog.git"
SENSOR_ANALOG_BRANCH="dev"  # Branch to use for SensorAnalog

# Clean up any existing consumer directory
if [ -d "$CONSUMERS_DIR" ]; then
  rm -rf "$CONSUMERS_DIR"
fi

# Create a fresh consumers directory
mkdir -p "$CONSUMERS_DIR"

# Clone UniversalMakerBox
echo "Cloning UniversalMakerBox into $CONSUMERS_DIR..."
git clone "$UNIVERSAL_MAKER_BOX_REPO" "$CONSUMERS_DIR/UniversalMakerBox"

# Navigate to the UniversalMakerBox directory
cd "$CONSUMERS_DIR/UniversalMakerBox" || exit

# Run prepare.sh to set up libraries (including default SensorAnalog)
echo "Running prepare.sh for UniversalMakerBox..."
bash prepare.sh

# Overwrite SensorAnalog with the dev branch version
echo "Overwriting SensorAnalog with dev branch..."
rm -rf "$HOME/Arduino/libraries/SensorAnalog"  # Remove the default SensorAnalog
git clone --branch "$SENSOR_ANALOG_BRANCH" "$SENSOR_ANALOG_REPO" "$HOME/Arduino/libraries/SensorAnalog"

# Now run the consumer's build and tests
echo "Building and running tests for UniversalMakerBox..."
bash build.sh    # Or the appropriate build command for the consumer

# Capture exit code to detect failure
if [ $? -ne 0 ]; then
  echo "Error: UniversalMakerBox build/tests failed."
  exit 1
fi

echo "Consumers prepared and tested successfully."

