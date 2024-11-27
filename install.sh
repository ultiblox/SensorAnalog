#!/bin/bash

# Load environment variables
source ./env.sh

# Ensure the Arduino libraries directory exists
if [ ! -d "$ARDUINO_LIBRARIES_DIR" ]; then
    echo "Arduino libraries directory not found. Creating it..."
    mkdir -p "$ARDUINO_LIBRARIES_DIR" || {
        echo "Error: Failed to create Arduino libraries directory."
        exit 1
    }
fi

# Install dependencies
DEPENDENCIES_FILE="./dependencies.txt"
while IFS=',' read -r DEP_NAME GIT_URL || [ -n "$DEP_NAME" ]; do
    DEP_NAME=$(echo "${DEP_NAME}" | tr -d '\r' | sed 's/^[ \t]*//;s/[ \t]*$//')
    GIT_URL=$(echo "${GIT_URL}" | tr -d '\r' | sed 's/^[ \t]*//;s/[ \t]*$//')

    if [[ -z "$DEP_NAME" || -z "$GIT_URL" ]]; then
        echo "Skipping empty or invalid line"
        continue
    fi

    TARGET_NAME="UltiBlox-$DEP_NAME"
    TARGET_PATH="$ARDUINO_LIBRARIES_DIR/$TARGET_NAME"

    if [ -d "$(dirname "$(pwd)")/$DEP_NAME" ]; then
        echo "Installing $DEP_NAME as $TARGET_NAME..."
        cp -r "$(dirname "$(pwd)")/$DEP_NAME" "$TARGET_PATH" || {
            echo "Error: Failed to install $DEP_NAME."
            continue
        }
    else
        echo "Warning: Dependency $DEP_NAME not found. Run prepare-libraries.sh first."
    fi
done < "$DEPENDENCIES_FILE"

# Install the main library
cp -r "$(pwd)" "$ARDUINO_LIBRARIES_DIR/$LIBRARY_NAME" || {
    echo "Error: Failed to install $LIBRARY_NAME."
    exit 1
}

echo "$LIBRARY_NAME and its dependencies installed successfully."