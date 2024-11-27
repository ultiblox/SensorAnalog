#!/bin/bash

# Load environment variables
source ./env.sh

# Ensure the Arduino libraries directory exists
mkdir -p "$ARDUINO_LIBRARIES_DIR"

# Create symlinks for dependencies
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
        echo "Creating symlink for $DEP_NAME as $TARGET_NAME..."
        ln -r -s "$(dirname "$(pwd)")/$DEP_NAME" "$TARGET_PATH" || {
            echo "Error: Failed to create symlink for $DEP_NAME."
            continue
        }
    else
        echo "Warning: Dependency $DEP_NAME not found. Run prepare-libraries.sh first."
    fi
done < "$DEPENDENCIES_FILE"

# Symlink the main library
LIBRARY_PATH="$PWD"
TARGET_PATH="$ARDUINO_LIBRARIES_DIR/$LIBRARY_NAME"

if [ -L "$TARGET_PATH" ] || [ -d "$TARGET_PATH" ]; then
    echo "Symlink or directory for $LIBRARY_NAME already exists."
else
    echo "Creating symlink for $LIBRARY_NAME..."
    ln -r -s "$LIBRARY_PATH" "$TARGET_PATH" || {
        echo "Error: Failed to create symlink for $LIBRARY_NAME."
        exit 1
    }
fi

echo "$LIBRARY_NAME and its dependencies symlinked successfully."