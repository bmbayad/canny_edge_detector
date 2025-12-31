#!/bin/bash

# Quick test script for Canny Edge Detector
# This script creates a test image and runs the edge detector

set -e

echo "=================================="
echo "Canny Edge Detector - Quick Test"
echo "=================================="
echo ""

# Check if executable exists
if [ ! -f "build/bin/canny_edge_detector" ]; then
    echo "Error: Executable not found. Please build the project first:"
    echo "  ./build.sh"
    exit 1
fi

# Create data directory if it doesn't exist
mkdir -p data

# Create a simple test image using Python
echo "Creating test image..."
python3 << 'EOF'
import numpy as np
from PIL import Image, ImageDraw

# Create a 512x512 image with some shapes
img = Image.new('RGB', (512, 512), 'white')
draw = ImageDraw.Draw(img)

# Draw some shapes for edge detection
draw.rectangle([50, 50, 200, 200], outline='black', width=3)
draw.ellipse([250, 50, 450, 250], outline='black', width=3)
draw.line([50, 300, 450, 300], fill='black', width=3)
draw.polygon([(250, 350), (150, 480), (350, 480)], outline='black', width=3)

# Save the image
img.save('data/test_shapes.jpg')
print("Test image created: data/test_shapes.jpg")
EOF

# Create output directory
mkdir -p output

echo ""
echo "Running edge detection..."
echo ""

# Run the edge detector
pushd build > /dev/null
./bin/canny_edge_detector -i ../data/test_shapes.jpg -o ../output/test_shapes
popd > /dev/null

echo ""
echo "=================================="
echo "Test complete!"
echo "=================================="
echo ""
echo "Check the output directory for results:"
echo "  output/test_shapes_cpu.png"
echo "  output/test_shapes_gpu.png"
echo ""
