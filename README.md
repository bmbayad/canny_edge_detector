# GPU Canny Edge Detection with OpenCV and CUDA

A modular C++ project demonstrating GPU-accelerated edge detection using OpenCV CUDA and CPU references, with comprehensive benchmarking, quality metrics, and flexible data configuration.

## Features

- **Dual Implementation**: CPU and GPU (CUDA) implementations of Canny edge detection
- **Comprehensive Benchmarking**: Detailed performance metrics with warmup runs and statistical analysis
- **Quality Metrics**: PSNR, SSIM, and pixel accuracy comparisons between CPU and GPU results
- **Flexible Configuration**: JSON-like text configuration file or command-line interface
- **Modular Design**: Clean separation of concerns with dedicated modules for each functionality
- **OpenCV CUDA**: Leverages OpenCV's CUDA module for GPU acceleration

## Project Structure

```
canny_edge_detector/
├── CMakeLists.txt           # CMake build configuration
├── config.txt               # Sample configuration file
├── include/                 # Header files
│   ├── benchmarking.hpp    # Benchmarking utilities
│   ├── canny_cpu.hpp       # CPU Canny implementation
│   ├── canny_gpu.hpp       # GPU Canny implementation
│   ├── config_loader.hpp   # Configuration file parser
│   └── quality_metrics.hpp # Quality metrics calculator
├── src/                     # Source files
│   ├── benchmarking.cpp
│   ├── canny_cpu.cpp
│   ├── canny_gpu.cpp
│   ├── config_loader.cpp
│   ├── main.cpp            # Main application
│   └── quality_metrics.cpp
├── data/                    # Input images directory
└── output/                  # Output images directory (created at runtime)
```

## Prerequisites

### Required Dependencies

- **CMake** 3.18 or higher
- **C++ Compiler** with C++17 support (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenCV** 4.x with CUDA support
- **CUDA Toolkit** 10.2 or higher
- **NVIDIA GPU** with compute capability 3.5 or higher

### Installing OpenCV with CUDA Support

#### Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake git pkg-config
sudo apt-get install libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install libv4l-dev libxvidcore-dev libx264-dev
sudo apt-get install libjpeg-dev libpng-dev libtiff-dev
sudo apt-get install gfortran openexr libatlas-base-dev
sudo apt-get install python3-dev python3-numpy

# Clone OpenCV and contrib modules
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

# Build OpenCV with CUDA
cd opencv
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D WITH_CUDA=ON \
      -D CUDA_ARCH_BIN=<your_gpu_compute_capability> \
      -D WITH_CUDNN=ON \
      -D OPENCV_DNN_CUDA=ON \
      -D ENABLE_FAST_MATH=1 \
      -D CUDA_FAST_MATH=1 \
      -D WITH_CUBLAS=1 \
      -D WITH_OPENGL=ON \
      -D BUILD_EXAMPLES=OFF ..

make -j$(nproc)
sudo make install
sudo ldconfig
```

Note: Replace `<your_gpu_compute_capability>` with your GPU's compute capability (e.g., 7.5 for RTX 2080, 8.6 for RTX 3090).

#### Verify OpenCV CUDA Installation

```bash
python3 -c "import cv2; print(cv2.cuda.getCudaEnabledDeviceCount())"
```

This should return a number greater than 0 if CUDA is properly configured.

## Building the Project

```bash
# Clone the repository
git clone https://github.com/bmbayad/canny_edge_detector.git
cd canny_edge_detector

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make

# The executable will be in build/bin/
./bin/canny_edge_detector --help
```

## Usage

### Command Line Interface

#### Basic Usage with Default Settings

```bash
# Process a single image with default parameters
./canny_edge_detector -i path/to/image.jpg

# Specify custom thresholds
./canny_edge_detector -i image.jpg -t1 50 -t2 150

# Use L2 gradient and custom aperture size
./canny_edge_detector -i image.jpg -t1 50 -t2 150 -a 5 -l2

# CPU only (no GPU processing)
./canny_edge_detector -i image.jpg --cpu-only

# GPU only (no CPU processing)
./canny_edge_detector -i image.jpg --gpu-only

# Skip benchmarking (faster processing)
./canny_edge_detector -i image.jpg --no-benchmark

# Custom output prefix
./canny_edge_detector -i image.jpg -o results/my_image
```

### Configuration File

Create a configuration file (e.g., `config.txt`) to process multiple images:

```ini
# Benchmark settings
[benchmark]
warmup_runs = 3
benchmark_runs = 10
save_outputs = true

# Image 1
[image]
input_path = data/image1.jpg
output_prefix = output/image1
threshold1 = 50.0
threshold2 = 150.0
aperture_size = 3
l2gradient = false

# Image 2
[image]
input_path = data/image2.jpg
output_prefix = output/image2
threshold1 = 100.0
threshold2 = 200.0
aperture_size = 5
l2gradient = true
```

Run with configuration file:

```bash
./canny_edge_detector -c config.txt
```

### Command Line Options

```
Options:
  -c, --config <file>     Path to configuration file
  -i, --input <file>      Input image file
  -o, --output <prefix>   Output file prefix (default: output)
  -t1 <value>             Canny threshold 1 (default: 50)
  -t2 <value>             Canny threshold 2 (default: 150)
  -a, --aperture <size>   Aperture size (3, 5, or 7; default: 3)
  -l2                     Use L2 gradient
  --cpu-only              Run CPU version only
  --gpu-only              Run GPU version only
  --no-benchmark          Skip benchmarking
  --no-save               Don't save output images
  -h, --help              Show help message
```

## Output

The program generates:

1. **Edge-detected images**: 
   - `<output_prefix>_cpu.png` - CPU processed image
   - `<output_prefix>_gpu.png` - GPU processed image

2. **Console output**:
   - CUDA device information
   - Processing parameters
   - Benchmark results (mean, min, max, standard deviation)
   - Speedup analysis (GPU vs CPU)
   - Quality metrics (PSNR, SSIM, pixel accuracy)

### Example Output

```
================================================================================
  GPU CANNY EDGE DETECTION - OpenCV CUDA Demo
================================================================================

CUDA Devices: 1
Device 0: NVIDIA GeForce RTX 3080
  Compute Capability: 8.6
  Total Memory: 10240 MB

Processing: data/sample.jpg
Parameters: threshold1=50, threshold2=150, aperture=3, L2gradient=false
Image size: 1920x1080

Running CPU Canny Edge Detection...
CPU result saved to: output_cpu.png

Running GPU Canny Edge Detection...
GPU result saved to: output_gpu.png

================================================================================
BENCHMARK RESULTS
================================================================================
Name                      Mean (ms)    Min (ms)    Max (ms)  StdDev (ms)
--------------------------------------------------------------------------------
CPU Canny                    45.234      43.127      48.901       1.823
GPU Canny (CUDA)              8.456       8.012       9.234       0.412
================================================================================

SPEEDUP ANALYSIS
--------------------------------------------------------------------------------
GPU Canny (CUDA) vs CPU Canny: 5.35x speedup
================================================================================

============================================================
QUALITY METRICS (CPU vs GPU)
============================================================
Metric                             Value
------------------------------------------------------------
PSNR (dB)                          48.23
SSIM                                0.9912
Pixel Accuracy (%)                 99.87
Matching Pixels                    2073600 / 2073600
============================================================
```

## Benchmarking

The benchmarking system includes:

- **Warmup runs**: To ensure GPU is initialized and caches are warm
- **Multiple benchmark runs**: For statistical significance
- **Statistical analysis**: Mean, min, max, and standard deviation
- **Speedup calculation**: Automatic comparison between CPU and GPU

## Quality Metrics

Three quality metrics are calculated to compare CPU and GPU outputs:

1. **PSNR (Peak Signal-to-Noise Ratio)**: Measures pixel-level differences (higher is better)
2. **SSIM (Structural Similarity Index)**: Measures structural similarity (0-1, higher is better)
3. **Pixel Accuracy**: Percentage of exactly matching pixels (0-100%)

High-quality implementations should show:
- PSNR > 40 dB
- SSIM > 0.95
- Pixel Accuracy > 95%

## Performance Considerations

### GPU Performance Tips

1. **Image Size**: Larger images benefit more from GPU acceleration
2. **Batch Processing**: Process multiple images for better GPU utilization
3. **Warmup**: Always perform warmup runs for accurate benchmarks
4. **Memory Transfer**: Data transfer time is included in benchmarks

### Expected Speedups

Typical speedups (GPU vs CPU):
- Small images (< 1MP): 2-3x
- Medium images (1-5MP): 4-6x
- Large images (> 5MP): 6-10x

Actual performance depends on:
- GPU model and compute capability
- CPU model and core count
- Image resolution
- OpenCV build configuration

## Troubleshooting

### CUDA Not Available

If you see "CUDA is not available":

1. Verify CUDA installation: `nvidia-smi`
2. Check OpenCV CUDA support: `python3 -c "import cv2; print(cv2.getBuildInformation())" | grep -i cuda`
3. Rebuild OpenCV with CUDA enabled (see installation instructions)

### Build Errors

- **CMake can't find OpenCV**: Set `OpenCV_DIR` environment variable
- **CUDA version mismatch**: Ensure CUDA toolkit version matches OpenCV build
- **Compute capability errors**: Adjust `CUDA_ARCH_BIN` in CMakeLists.txt

### Performance Issues

- Ensure GPU drivers are up to date
- Check GPU is not being used by other processes
- Increase benchmark runs for more stable measurements
- Verify power management settings (GPU should not be throttled)

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is provided as-is for educational and research purposes.

## Acknowledgments

- OpenCV community for CUDA module development
- NVIDIA for CUDA toolkit and GPU computing platform

## References

- [OpenCV CUDA Module Documentation](https://docs.opencv.org/master/d1/d1a/namespacecv_1_1cuda.html)
- [Canny Edge Detection Paper](https://doi.org/10.1109/TPAMI.1986.4767851)
- [CUDA Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)