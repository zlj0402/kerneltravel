#!/bin/bash

# Build script for CMake projects (supports both single and multi-config generators)
# Usage: ./build.sh [options]
#   -g, --generator GEN  CMake generator (default: Ninja, or MinGW Makefiles)
#                        Multi-config: "Ninja Multi-Config", "Visual Studio 17 2022", "Xcode"
#   -t, --type TYPE      Build type: Debug, Release, RelWithDebInfo, MinSizeRel (default: Debug)
#                        For multi-config generators, this is used at build time via --config
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
            cat << 'EOF'
Build script for CMake projects (supports both single and multi-config generators)

Usage: ./build.sh [options]

Options:
  -g, --generator GEN  CMake generator (default: Ninja, or MinGW Makefiles)
                       Multi-config: "Ninja Multi-Config", "Visual Studio 17 2022", "Xcode"
  -t, --type TYPE      Build type: Debug, Release, RelWithDebInfo, MinSizeRel (default: Debug)
                       For multi-config generators, this is used at build time via --config
  -i, --install        Install after building (to ./output)
  -c, --clean          Clean before building
  -j, --jobs N         Number of parallel jobs (default: auto)
  -h, --help           Show this help message

Examples:
  # Single-config (default Ninja)
  ./build.sh
  ./build.sh -t Release
  ./build.sh -g "MinGW Makefiles" -t Debug

  # Multi-config - build Debug
  ./build.sh -g "Ninja Multi-Config" -t Debug

  # Multi-config - build Release
  ./build.sh -g "Ninja Multi-Config" -t Release

  # Multi-config with install
  ./build.sh -g "Ninja Multi-Config" -t Release -i

  # Clean, build, and install
  ./build.sh -c -t Release -i

  # Build with parallel jobs
  ./build.sh -j 8 -t Debug
EOF
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

# Detect if generator is multi-config
is_multi_config() {
    case "$GENERATOR" in
        "Ninja Multi-Config"|"Visual Studio"*|"Xcode")
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Clean if requested
if [ "$CLEAN_FIRST" = true ]; then
    echo "=== Cleaning build directories ==="
    ./clean.sh "$BUILD_DIR"
    echo ""
fi

# Configure
echo "=== Configuring (${GENERATOR}, ${BUILD_TYPE}) ==="
if is_multi_config; then
    echo "Using multi-config generator - CMAKE_BUILD_TYPE will be set at build time"
    cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
else
    echo "Using single-config generator - setting CMAKE_BUILD_TYPE at configure time"
    cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
fi
echo ""

# Build
echo "=== Building ==="
if is_multi_config; then
    cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" $JOBS
else
    cmake --build "$BUILD_DIR" $JOBS
fi
echo ""

# Install if requested
if [ "$DO_INSTALL" = true ]; then
    echo "=== Installing to ${INSTALL_DIR} ==="
    if is_multi_config; then
        cmake --install "$BUILD_DIR" --config "$BUILD_TYPE"
    else
        cmake --install "$BUILD_DIR"
    fi
    echo ""
    echo "✓ Install complete"
fi

echo "✓ Build complete"
