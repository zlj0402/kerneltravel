#!/bin/bash

# Clean script for CMake build directory
# Usage: ./clean.sh [build_dir]

BUILD_DIR="${1:-build}"

# Directories to clean
DIRS_TO_CLEAN=(
    "$BUILD_DIR"
    "output"
    "Testing"
)

for dir in "${DIRS_TO_CLEAN[@]}"; do
    if [ -d "$dir" ]; then
        echo "Removing directory: $dir"
        rm -rf "$dir"
        if [ $? -eq 0 ]; then
            echo "✓ $dir removed successfully"
        else
            echo "✗ Failed to remove $dir"
        fi
    else
        echo "Directory '$dir' does not exist, skipping"
    fi
done
