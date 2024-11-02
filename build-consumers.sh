#!/bin/bash

# Directory for consumers
CONSUMERS_DIR="$HOME/ci-consumers"
UNIVERSAL_MAKER_BOX_REPO="https://github.com/ultiblox/UltiBloxTemplates.git"

# Clean up any existing consumer directory
if [ -d "$CONSUMERS_DIR" ]; then
  rm -rf "$CONSUMERS_DIR"
fi

# Create a fresh consumers directory
mkdir -p "$CONSUMERS_DIR"

# Clone UltiBloxTemplates
echo "Cloning UltiBloxTemplates into $CONSUMERS_DIR..."
git clone "$UNIVERSAL_MAKER_BOX_REPO" "$CONSUMERS_DIR/UltiBloxTemplates"

# Navigate to the UltiBloxTemplates directory
cd "$CONSUMERS_DIR/UltiBloxTemplates" || exit

# Run the build and tests for UniversalMakerBox
echo "Building and running tests for UltiBloxTemplates..."
bash prepare.sh
bash build.sh    # Or the appropriate build command for the consumer

# Capture exit code to detect failure
if [ $? -ne 0 ]; then
  echo "Error: UltiBloxTemplates build/tests failed."
  exit 1
fi

echo "Consumers prepared and tested successfully."
