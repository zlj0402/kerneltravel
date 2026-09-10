#!/bin/bash

# Build script for CMake projects (single-config generators)
# Usage: ./build.sh [options]
#   -g, --generator GEN  CMake generator (default: Ninja, or MinGW Makefiles)
#   -t, --type TYPE      Build type: Debug, Release, RelWithDebInfo, MinSizeRel (default: Debug)
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
        -c|--clean)
            CLEAN_FIRST=true
            shift
            ;;
        -j|--jobs)
            JOBS="--parallel $2"
            shift 2
            ;;
        -h|--help)
            head -n 8 "$0" | tail -n 7
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

BUILD_DIR="build"

# Clean if requested
if [ "$CLEAN_FIRST" = true ]; then
    echo "=== Cleaning build directories ==="
    ./clean.sh "$BUILD_DIR"
    echo ""
fi

# Configure
echo "=== Configuring (${GENERATOR}, ${BUILD_TYPE}) ==="
cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
echo ""

# Build
echo "=== Building ==="
cmake --build "$BUILD_DIR" $JOBS
echo ""

echo "✓ Build complete"
