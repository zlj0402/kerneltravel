#!/bin/bash

# Build script for CMake projects (single-config generators)
# Usage: ./build.sh [options]
#   -g, --generator GEN  CMake generator (default: Ninja, or MinGW Makefiles)
#   -t, --type TYPE      Build type: Debug, Release, RelWithDebInfo, MinSizeRel (default: Debug)
#   -i, --install        Install after building (to ./output)
#   -c, --clean          Clean before building
#   -j, --jobs N         Number of parallel jobs (default: auto)
#   -h, --help           Show this help message

set -e

# Auto-detect generator
if command -v ninja &> /dev/null; then
    DEFAULT_GENERATOR="Ninja"
else
    DEFAULT_GENERATOR="MinGW Makefiles"
fi

GENERATOR="$DEFAULT_GENERATOR"
BUILD_TYPE="Debug"
DO_INSTALL=false
CLEAN_FIRST=false
JOBS=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -g|--generator)
            GENERATOR="$2"
            shift 2
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -i|--install)
            DO_INSTALL=true
            shift
            ;;
        -c|--clean)
            CLEAN_FIRST=true
            shift
            ;;
        -j|--jobs)
            JOBS="--parallel $2"
            shift 2
            ;;
        -h|--help)
            head -n 9 "$0" | tail -n 8
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

BUILD_DIR="build"
INSTALL_DIR="output"

# Clean if requested
if [ "$CLEAN_FIRST" = true ]; then
    echo "=== Cleaning build directories ==="
    ./clean.sh "$BUILD_DIR"
    echo ""
fi

# Configure
echo "=== Configuring (${GENERATOR}, ${BUILD_TYPE}) ==="
cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
echo ""

# Build
echo "=== Building ==="
cmake --build "$BUILD_DIR" $JOBS
echo ""

# Install if requested
if [ "$DO_INSTALL" = true ]; then
    echo "=== Installing to ${INSTALL_DIR} ==="
    cmake --install "$BUILD_DIR"
    echo ""
    echo "✓ Install complete"
fi

echo "✓ Build complete"
