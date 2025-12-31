#!/bin/bash

# Build script for Canny Edge Detector

set -e

echo "=================================="
echo "Building Canny Edge Detector"
echo "=================================="

# Check if build directory exists
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo "Running CMake configuration..."
cmake ..

# Build the project
echo "Building project..."
# Determine number of cores for parallel build
if command -v nproc > /dev/null 2>&1; then
    CORES=$(nproc)
elif command -v sysctl > /dev/null 2>&1; then
    CORES=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
else
    CORES=4
fi
make -j${CORES}

echo ""
echo "=================================="
echo "Build complete!"
echo "=================================="
echo ""
echo "Executable location: build/bin/canny_edge_detector"
echo ""
echo "To run with default settings:"
echo "  cd build"
echo "  ./bin/canny_edge_detector --help"
echo ""
echo "To test (if you have an image):"
echo "  ./bin/canny_edge_detector -i /path/to/image.jpg"
echo ""
