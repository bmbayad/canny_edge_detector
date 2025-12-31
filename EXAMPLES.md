# Examples

This document provides various usage examples for the Canny Edge Detector.

## Table of Contents

- [Basic Examples](#basic-examples)
- [Advanced Usage](#advanced-usage)
- [Configuration File Examples](#configuration-file-examples)
- [Batch Processing](#batch-processing)
- [Performance Tuning](#performance-tuning)

## Basic Examples

### Example 1: Default Settings

Process an image with default Canny parameters:

```bash
./canny_edge_detector -i data/image.jpg
```

Output:
- `output_cpu.png` - CPU processed result
- `output_gpu.png` - GPU processed result
- Console: Benchmark results and quality metrics

### Example 2: Custom Thresholds

Adjust Canny thresholds for different edge sensitivity:

```bash
# Lower thresholds = more edges (noisier)
./canny_edge_detector -i data/image.jpg -t1 30 -t2 90

# Higher thresholds = fewer edges (cleaner)
./canny_edge_detector -i data/image.jpg -t1 100 -t2 200
```

### Example 3: Custom Output Location

Specify output file prefix:

```bash
./canny_edge_detector -i data/photo.jpg -o results/photo_edges
```

Output:
- `results/photo_edges_cpu.png`
- `results/photo_edges_gpu.png`

### Example 4: CPU Only Mode

Useful when GPU is not available or for comparison:

```bash
./canny_edge_detector -i data/image.jpg --cpu-only
```

### Example 5: GPU Only Mode

Skip CPU processing for faster results:

```bash
./canny_edge_detector -i data/image.jpg --gpu-only
```

## Advanced Usage

### Example 6: L2 Gradient

Use L2 norm for gradient magnitude calculation:

```bash
./canny_edge_detector -i data/image.jpg -l2
```

The L2 gradient provides more accurate gradient magnitude:
- Standard: `|dI/dx| + |dI/dy|`
- L2: `sqrt((dI/dx)^2 + (dI/dy)^2)`

### Example 7: Custom Aperture Size

Adjust Sobel kernel size (3, 5, or 7):

```bash
# Smaller aperture = more detailed edges
./canny_edge_detector -i data/image.jpg -a 3

# Larger aperture = smoother edges
./canny_edge_detector -i data/image.jpg -a 7
```

### Example 8: Combined Parameters

Use multiple parameters together:

```bash
./canny_edge_detector -i data/landscape.jpg \
    -t1 50 -t2 150 \
    -a 5 \
    -l2 \
    -o results/landscape_detailed
```

### Example 9: Skip Benchmarking

For faster processing without performance measurements:

```bash
./canny_edge_detector -i data/image.jpg --no-benchmark
```

### Example 10: Process Without Saving

Test parameters without saving output files:

```bash
./canny_edge_detector -i data/image.jpg --no-save
```

## Configuration File Examples

### Example 11: Basic Configuration

Create `config_basic.txt`:

```ini
[benchmark]
warmup_runs = 3
benchmark_runs = 10
save_outputs = true

[image]
input_path = data/photo1.jpg
output_prefix = output/photo1
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 3
l2gradient = false
```

Run:

```bash
./canny_edge_detector -c config_basic.txt
```

### Example 12: Multiple Images

Create `config_batch.txt`:

```ini
[benchmark]
warmup_runs = 5
benchmark_runs = 20
save_outputs = true

[image]
input_path = data/portrait.jpg
output_prefix = output/portrait
threshold1 = 80.0
threshold2 = 180.0
aperture_size = 3
l2gradient = false

[image]
input_path = data/landscape.jpg
output_prefix = output/landscape
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 5
l2gradient = true

[image]
input_path = data/architecture.jpg
output_prefix = output/architecture
threshold1 = 100.0
threshold2 = 200.0
aperture_size = 3
l2gradient = true
```

Run:

```bash
./canny_edge_detector -c config_batch.txt
```

### Example 13: Different Parameter Sets

Test different parameters on the same image:

```ini
[benchmark]
warmup_runs = 3
benchmark_runs = 10
save_outputs = true

[image]
input_path = data/test.jpg
output_prefix = output/test_low_threshold
threshold1 = 30.0
threshold2 = 90.0
aperture_size = 3
l2gradient = false

[image]
input_path = data/test.jpg
output_prefix = output/test_medium_threshold
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 3
l2gradient = false

[image]
input_path = data/test.jpg
output_prefix = output/test_high_threshold
threshold1 = 100.0
threshold2 = 200.0
aperture_size = 3
l2gradient = false
```

## Batch Processing

### Example 14: Process Directory of Images

Create a bash script `process_all.sh`:

```bash
#!/bin/bash

mkdir -p output

for image in data/*.jpg; do
    basename=$(basename "$image" .jpg)
    echo "Processing $basename..."
    ./build/bin/canny_edge_detector \
        -i "$image" \
        -o "output/$basename" \
        -t1 50 -t2 150
done

echo "All images processed!"
```

Run:

```bash
chmod +x process_all.sh
./process_all.sh
```

### Example 15: Parameter Sweep

Test different threshold combinations:

```bash
#!/bin/bash

image="data/test.jpg"

for t1 in 30 50 70 100; do
    t2=$((t1 * 3))
    echo "Testing thresholds: $t1, $t2"
    ./build/bin/canny_edge_detector \
        -i "$image" \
        -o "output/sweep_${t1}_${t2}" \
        -t1 $t1 -t2 $t2 \
        --no-benchmark
done
```

## Performance Tuning

### Example 16: Extended Benchmarking

For more accurate performance measurements:

Create `config_benchmark.txt`:

```ini
[benchmark]
warmup_runs = 10
benchmark_runs = 50
save_outputs = false

[image]
input_path = data/large_image.jpg
output_prefix = output/benchmark
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 3
l2gradient = false
```

### Example 17: Resolution Testing

Test performance across different resolutions:

```bash
#!/bin/bash

for size in 512 1024 2048 4096; do
    echo "Testing ${size}x${size}..."
    
    # Create test image of specific size (requires ImageMagick)
    convert data/original.jpg -resize ${size}x${size} data/test_${size}.jpg
    
    # Run benchmark
    ./build/bin/canny_edge_detector \
        -i data/test_${size}.jpg \
        -o output/test_${size} \
        --no-save
done
```

## Real-World Scenarios

### Example 18: Video Frame Processing

Extract and process frames from a video:

```bash
#!/bin/bash

# Extract frames from video (requires ffmpeg)
ffmpeg -i input_video.mp4 -vf fps=1 data/frame_%04d.jpg

# Process all frames
for frame in data/frame_*.jpg; do
    basename=$(basename "$frame" .jpg)
    ./build/bin/canny_edge_detector \
        -i "$frame" \
        -o "output/$basename" \
        --gpu-only \
        --no-benchmark
done

# Create video from processed frames
ffmpeg -framerate 1 -i output/frame_%04d_gpu.png output_video.mp4
```

### Example 19: Medical Image Processing

Process medical images with specific parameters:

```bash
# X-ray image - high detail
./canny_edge_detector \
    -i data/xray.jpg \
    -t1 100 -t2 200 \
    -a 3 \
    -o results/xray_edges

# MRI scan - smoother edges
./canny_edge_detector \
    -i data/mri.jpg \
    -t1 50 -t2 120 \
    -a 5 \
    -l2 \
    -o results/mri_edges
```

### Example 20: Document Edge Detection

Process document scans:

```bash
# High contrast for clear document edges
./canny_edge_detector \
    -i data/document.jpg \
    -t1 150 -t2 250 \
    -a 3 \
    -o results/document_edges
```

## Tips and Best Practices

### Threshold Selection

- **General photos**: t1=50, t2=150
- **High contrast**: t1=100, t2=200
- **Low contrast**: t1=30, t2=90
- **Noisy images**: Higher thresholds (t1=80, t2=200)

### Aperture Size Selection

- **aperture=3**: Fast, detailed edges, more noise
- **aperture=5**: Balanced, most common
- **aperture=7**: Slower, smooth edges, less noise

### L2 Gradient

- Use L2 gradient (`-l2`) for:
  - More accurate edge detection
  - Scientific applications
  - When quality is more important than speed

### Performance Optimization

- Use `--gpu-only` for production pipelines
- Use `--no-benchmark` when not measuring performance
- Use `--no-save` for parameter tuning
- Process multiple images in batch for better GPU utilization

## Getting Help

For more information:

```bash
# Show all options
./canny_edge_detector --help

# Check CUDA availability
./canny_edge_detector | grep -i cuda
```

## Additional Resources

- See `README.md` for complete documentation
- See `BUILD.md` for build instructions
- Check OpenCV documentation for parameter details
