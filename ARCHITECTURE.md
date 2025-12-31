# Architecture Overview

This document describes the architecture and design decisions of the GPU Canny Edge Detection project.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         Main Application                      │
│                          (main.cpp)                           │
└──────┬──────────────────────────────────────────────────┬───┘
       │                                                   │
       │                                                   │
┌──────▼─────────┐                              ┌────────▼────────┐
│ Config Loader  │                              │  Command Line   │
│                │                              │     Parser      │
└──────┬─────────┘                              └────────┬────────┘
       │                                                  │
       └────────────────────┬─────────────────────────────┘
                            │
              ┌─────────────▼──────────────┐
              │  Processing Controller     │
              │  - Load images            │
              │  - Coordinate processing  │
              │  - Manage output          │
              └─────────┬────────┬─────────┘
                        │        │
           ┌────────────┘        └────────────┐
           │                                  │
    ┌──────▼──────┐                   ┌──────▼──────┐
    │  CPU Canny  │                   │  GPU Canny  │
    │  Processor  │                   │  Processor  │
    │             │                   │   (CUDA)    │
    └──────┬──────┘                   └──────┬──────┘
           │                                  │
           └────────────┬─────────────────────┘
                        │
           ┌────────────▼──────────────┐
           │    Results & Metrics      │
           │  - Benchmarking          │
           │  - Quality Metrics       │
           │  - Output Generation     │
           └───────────────────────────┘
```

## Module Design

### 1. Configuration Module (`config_loader.hpp/cpp`)

**Purpose**: Parse configuration files and manage application settings.

**Key Features**:
- Simple INI-style configuration format
- Support for multiple image configurations
- Default configuration fallback
- Validation and error handling

**Design Decisions**:
- Text-based format for easy editing
- Section-based organization
- Flexible parameter specification

### 2. CPU Canny Module (`canny_cpu.hpp/cpp`)

**Purpose**: Implement CPU-based Canny edge detection.

**Key Features**:
- OpenCV's standard Canny implementation
- Configurable parameters (thresholds, aperture, L2 gradient)
- Image loading and saving utilities

**Design Decisions**:
- Static class methods for stateless operation
- Separate detect() and processImage() methods
- Automatic grayscale conversion

**Algorithm Flow**:
```
Input Image
    ↓
Grayscale Conversion (if needed)
    ↓
Gaussian Blur (implicit in OpenCV)
    ↓
Gradient Calculation (Sobel)
    ↓
Non-maximum Suppression
    ↓
Double Threshold
    ↓
Edge Tracking by Hysteresis
    ↓
Output Edge Map
```

### 3. GPU Canny Module (`canny_gpu.hpp/cpp`)

**Purpose**: Implement GPU-accelerated Canny edge detection using CUDA.

**Key Features**:
- CUDA availability detection
- GPU device information retrieval
- Parallel edge detection on GPU
- Efficient memory transfer

**Design Decisions**:
- Mirror CPU interface for consistency
- CUDA stream management handled by OpenCV
- Graceful degradation when CUDA unavailable
- Memory transfer time included in benchmarks

**CUDA Pipeline**:
```
Host (CPU)           GPU Device
    │                    │
    │ Upload Image      │
    ├──────────────────►│
    │                    │
    │              Convert to Gray
    │                    │
    │              Gaussian Blur
    │                    │
    │              Sobel Gradients
    │                    │
    │              Non-max Suppression
    │                    │
    │              Hysteresis
    │                    │
    │ Download Result   │
    │◄──────────────────┤
    │                    │
```

### 4. Benchmarking Module (`benchmarking.hpp/cpp`)

**Purpose**: Measure and compare performance of CPU and GPU implementations.

**Key Features**:
- Warmup runs to initialize GPU and caches
- Multiple benchmark runs for statistical significance
- Mean, min, max, and standard deviation calculations
- Speedup analysis

**Design Decisions**:
- High-resolution timer for accuracy
- Separate warmup phase
- Statistical analysis for reliability
- Formatted output for readability

**Benchmark Process**:
```
1. Warmup Phase (3-10 runs)
   - Initialize GPU
   - Warm up caches
   - Stabilize system

2. Measurement Phase (10-50 runs)
   - Time each execution
   - Record all measurements

3. Analysis Phase
   - Calculate statistics
   - Compare implementations
   - Report results
```

### 5. Quality Metrics Module (`quality_metrics.hpp/cpp`)

**Purpose**: Assess quality and accuracy of edge detection results.

**Key Features**:
- PSNR (Peak Signal-to-Noise Ratio)
- SSIM (Structural Similarity Index)
- Pixel accuracy calculation
- Formatted metric reporting

**Design Decisions**:
- Multiple complementary metrics
- Standard image quality measures
- Pixel-perfect comparison support

**Metrics Explanation**:

1. **PSNR**: Measures pixel-level differences
   - Higher is better (>40 dB is excellent)
   - Formula: `10 * log10(MAX^2 / MSE)`

2. **SSIM**: Measures structural similarity
   - Range: 0 to 1 (1 is identical)
   - Considers luminance, contrast, structure
   - More perceptually relevant than PSNR

3. **Pixel Accuracy**: Exact match percentage
   - Simple and intuitive
   - 100% means identical results

### 6. Main Application (`main.cpp`)

**Purpose**: Orchestrate all modules and provide user interface.

**Key Features**:
- Command-line argument parsing
- Dual-mode operation (config file or CLI)
- Progress reporting
- Error handling

**Design Decisions**:
- Flexible interface (CLI or config file)
- Comprehensive help system
- Graceful error handling
- Clear output formatting

## Data Flow

```
┌──────────────┐
│Configuration │
│  or CLI Args │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ Load Config  │
└──────┬───────┘
       │
       ▼
┌──────────────┐     ┌──────────────┐
│ Load Image   │────►│   Process    │
└──────────────┘     │  CPU/GPU     │
                     └──────┬───────┘
                            │
              ┌─────────────┴─────────────┐
              │                           │
              ▼                           ▼
       ┌─────────────┐            ┌─────────────┐
       │  Benchmark  │            │   Quality   │
       │   Results   │            │   Metrics   │
       └──────┬──────┘            └──────┬──────┘
              │                           │
              └─────────────┬─────────────┘
                            │
                            ▼
                     ┌─────────────┐
                     │   Display   │
                     │  & Save     │
                     └─────────────┘
```

## Memory Management

### CPU Memory
- OpenCV Mat objects with automatic memory management
- Copy-on-write semantics for efficiency
- RAII for resource cleanup

### GPU Memory
- OpenCV GpuMat for CUDA memory
- Automatic allocation/deallocation
- Stream-based processing for efficiency

### Memory Transfer
```
CPU Memory          GPU Memory
   ┌────┐              ┌────┐
   │Mat │──upload()──►│GpuMat│
   └────┘              └────┘
      ▲                  │
      │                  │
      │   download()     │
      └──────────────────┘
```

## Performance Considerations

### CPU Optimization
- OpenCV uses SIMD instructions (SSE, AVX)
- Multi-threading for parallel operations
- Cache-efficient memory access

### GPU Optimization
- Parallel processing of pixels
- Shared memory for kernel optimization
- Coalesced memory access patterns
- Asynchronous execution

### Bottlenecks
1. **Memory Transfer**: CPU↔GPU communication overhead
2. **Small Images**: GPU overhead not amortized
3. **Sequential Operations**: Limited parallelism

## Error Handling

```
┌─────────────────┐
│  Operation      │
└────────┬────────┘
         │
         ▼
    Success?
         │
    ┌────┴────┐
    │         │
   Yes        No
    │         │
    ▼         ▼
Continue  ┌──────────┐
          │  Error   │
          │ Handler  │
          └────┬─────┘
               │
          ┌────┴─────┐
          │          │
     Recoverable   Fatal
          │          │
          ▼          ▼
      Log &      Exit with
      Continue   Error Code
```

## Testing Strategy

### Unit Testing (Conceptual)
- Individual module testing
- Mock dependencies
- Edge case validation

### Integration Testing
- End-to-end workflow testing
- Multiple configuration scenarios
- Error handling verification

### Performance Testing
- Benchmark consistency
- Memory leak detection
- Scalability testing

## Extensibility

### Adding New Features

1. **New Quality Metrics**:
   - Add method to `QualityMetricsCalculator`
   - Update `QualityMetrics` struct
   - Update `printMetrics()`

2. **New Processing Methods**:
   - Create new module (e.g., `canny_opencl.hpp/cpp`)
   - Follow existing interface pattern
   - Integrate in `main.cpp`

3. **New Configuration Options**:
   - Update `Config` structs in `config_loader.hpp`
   - Update parser in `config_loader.cpp`
   - Add CLI arguments in `main.cpp`

## Dependencies

### Required
- **CMake**: Build system
- **OpenCV**: Image processing and CUDA wrappers
- **CUDA Toolkit**: GPU computing

### Optional
- **ImageMagick**: Test image generation
- **FFmpeg**: Video processing examples

## Build System

CMake Configuration Flow:
```
CMakeLists.txt
     │
     ├──Find OpenCV
     ├──Find CUDA
     ├──Check CUDA support
     ├──Set compiler flags
     ├──Add include directories
     ├──Add source files
     ├──Link libraries
     └──Set output directory
```

## Future Enhancements

### Potential Improvements
1. **Additional Backends**: OpenCL, Vulkan, Metal
2. **Batch Processing**: Multiple images simultaneously
3. **Video Support**: Real-time video edge detection
4. **GUI**: Visual parameter tuning
5. **Python Bindings**: Use from Python
6. **Docker Container**: Easy deployment
7. **CI/CD**: Automated testing and builds

### Performance Optimizations
1. **Custom CUDA Kernels**: Direct CUDA implementation
2. **Zero-Copy Memory**: Unified memory for efficiency
3. **Stream Pipelining**: Overlap computation and transfer
4. **Multi-GPU**: Distribute work across GPUs

## References

- [OpenCV Documentation](https://docs.opencv.org/)
- [CUDA Programming Guide](https://docs.nvidia.com/cuda/)
- [Canny Edge Detection Algorithm](https://en.wikipedia.org/wiki/Canny_edge_detector)
- [CMake Documentation](https://cmake.org/documentation/)
