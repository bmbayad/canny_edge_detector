# Quick Reference

Quick reference guide for the GPU Canny Edge Detector project.

## One-Minute Start

```bash
# Build
./build.sh

# Run with test image
./build/bin/canny_edge_detector -i your_image.jpg

# Or use config file
./build/bin/canny_edge_detector -c config.txt
```

## Common Commands

### Building

```bash
# Standard build
./build.sh

# Manual build
mkdir build && cd build
cmake .. && make -j$(nproc)

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
```

### Running

```bash
# Basic usage
./canny_edge_detector -i image.jpg

# Custom thresholds
./canny_edge_detector -i image.jpg -t1 50 -t2 150

# CPU only
./canny_edge_detector -i image.jpg --cpu-only

# GPU only
./canny_edge_detector -i image.jpg --gpu-only

# With config file
./canny_edge_detector -c config.txt

# Help
./canny_edge_detector --help
```

## File Structure

```
.
├── src/                    # Source files (.cpp)
├── include/                # Header files (.hpp)
├── data/                   # Input images
├── output/                 # Output images (created)
├── build/                  # Build directory (created)
├── CMakeLists.txt         # Build configuration
├── config.txt             # Sample configuration
├── build.sh               # Build script
└── test.sh                # Test script
```

## Key Files

| File | Purpose |
|------|---------|
| `README.md` | Main documentation |
| `BUILD.md` | Build instructions |
| `EXAMPLES.md` | Usage examples |
| `ARCHITECTURE.md` | System design |
| `CONTRIBUTING.md` | How to contribute |
| `CHANGELOG.md` | Version history |
| `AUTHORS.md` | Contributors |

## Configuration Format

```ini
[benchmark]
warmup_runs = 3
benchmark_runs = 10
save_outputs = true

[image]
input_path = data/image.jpg
output_prefix = output/image
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 3
l2gradient = false
```

## CLI Options Quick Reference

| Option | Description | Example |
|--------|-------------|---------|
| `-i, --input` | Input image | `-i image.jpg` |
| `-o, --output` | Output prefix | `-o results/img` |
| `-t1` | Threshold 1 | `-t1 50` |
| `-t2` | Threshold 2 | `-t2 150` |
| `-a, --aperture` | Aperture size | `-a 5` |
| `-l2` | Use L2 gradient | `-l2` |
| `-c, --config` | Config file | `-c config.txt` |
| `--cpu-only` | CPU only | `--cpu-only` |
| `--gpu-only` | GPU only | `--gpu-only` |
| `--no-benchmark` | Skip benchmark | `--no-benchmark` |
| `--no-save` | Don't save output | `--no-save` |
| `-h, --help` | Show help | `--help` |

## Parameter Guidelines

### Canny Thresholds

| Use Case | threshold1 | threshold2 |
|----------|-----------|------------|
| General photos | 50 | 150 |
| High contrast | 100 | 200 |
| Low contrast | 30 | 90 |
| Noisy images | 80 | 200 |

### Aperture Size

- `3`: Fast, detailed, more noise
- `5`: Balanced (recommended)
- `7`: Slow, smooth, less noise

## Output Files

- `<prefix>_cpu.png` - CPU result
- `<prefix>_gpu.png` - GPU result

## Troubleshooting

### CUDA not available
```bash
# Check GPU
nvidia-smi

# Check OpenCV CUDA
python3 -c "import cv2; print(cv2.cuda.getCudaEnabledDeviceCount())"
```

### Build fails
```bash
# Specify OpenCV path
cmake -DOpenCV_DIR=/path/to/opencv/build ..
```

### Can't find image
```bash
# Use absolute path
./canny_edge_detector -i /full/path/to/image.jpg

# Or relative from executable location
cd build
./bin/canny_edge_detector -i ../data/image.jpg
```

## Performance Tips

1. Use GPU for large images (> 1MP)
2. Skip benchmarking for faster processing (`--no-benchmark`)
3. Use configuration file for batch processing
4. Process multiple images to amortize GPU initialization

## Quality Metrics Interpretation

| Metric | Good | Excellent |
|--------|------|-----------|
| PSNR | > 35 dB | > 40 dB |
| SSIM | > 0.90 | > 0.95 |
| Pixel Accuracy | > 90% | > 95% |

## Example Workflows

### Single Image Processing
```bash
./canny_edge_detector -i photo.jpg -t1 50 -t2 150
```

### Batch Processing
```bash
# Create config.txt with multiple [image] sections
./canny_edge_detector -c config.txt
```

### Parameter Tuning
```bash
# Try different thresholds
for t1 in 30 50 70; do
    ./canny_edge_detector -i test.jpg -t1 $t1 -t2 $((t1*3)) \
        -o output/test_$t1 --no-benchmark
done
```

### Performance Comparison
```bash
# Compare CPU vs GPU
./canny_edge_detector -i large_image.jpg
# Check benchmark output
```

## Getting Help

- `--help` - Built-in help
- `README.md` - Complete documentation
- `EXAMPLES.md` - Usage examples
- Issues - https://github.com/bmbayad/canny_edge_detector/issues

## Links

- **Repository**: https://github.com/bmbayad/canny_edge_detector
- **OpenCV CUDA**: https://docs.opencv.org/master/d1/d1a/namespacecv_1_1cuda.html
- **Canny Algorithm**: https://en.wikipedia.org/wiki/Canny_edge_detector
