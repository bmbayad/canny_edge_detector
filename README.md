# GPU-Accelerated Canny Edge Detector

This repository provides a small benchmark that compares a CPU implementation
of the Canny edge detector against an OpenCV CUDA implementation. It
processes all images in `data/input`, writes edge images to `data/output`, and
appends benchmarking rows to `data/results/benchmark_all.csv`.

Key behaviors:
- Runs `gpu::runCanny` (OpenCV CUDA) and `cpu::runCanny` (OpenCV CPU) on
  each grayscale input image.
- Measures per-image runtime (milliseconds) for GPU and CPU.
- Computes simple quality metrics comparing the two edge images:
  - Pixel-wise match percentage
  - Edge Intersection-over-Union (IoU)
- Saves visual outputs and a CSV summary suitable for later analysis.

## Repository layout

- `CMakeLists.txt` - build configuration
- `src/` - implementation sources
  - `canny_cpu.h/.cpp` - CPU wrapper for OpenCV Canny
  - `canny_cuda.h/.cpp` - GPU wrapper using OpenCV CUDA Canny
  - `image_io.h/.cpp` - helpers to load/save images
  - `metrics.h/.cpp` - pixel match & edge IoU calculations
  - `timing.h/.cpp` - simple high-resolution timer
  - `main.cpp` - benchmark runner (iterates `data/input` → `data/output`)
- `data/` - sample I/O layout
  - `input/` (place your test images here)
  - `output/` (edge images are written here)
  - `results/benchmark_all.csv` (generated CSV summary)

## CSV Columns

| Column | Description | Unit/Range |
|---|---|---|
| Filename | Input image filename | string |
| CPU_ms | CPU Canny runtime | milliseconds |
| GPU_ms | GPU Canny runtime | milliseconds |
| Speedup | `CPU_ms / GPU_ms` | ratio (dimensionless) |
| PixelMatch | Percentage of identical pixels between CPU/GPU outputs | 0–100% |
| EdgeIoU | Intersection-over-Union for detected edge pixels | 0–100% |

## Dependencies

- CMake (>= 3.10)
- A C++17-compatible compiler (Visual Studio / clang / gcc)
- OpenCV built with CUDA support for GPU runs. The GPU path will fail if
  OpenCV CUDA modules are not available; the CPU path only requires core OpenCV.

Recommended OpenCV modules: `core`, `imgproc`, `highgui`, `cudaimgproc`.

## Building

Typical out-of-source build with CMake:

Windows (MSVC):

```powershell
mkdir build
cd build
cmake -G "Visual Studio 17 2022" .. -A x64 -DOpenCV_DIR="C:/path/to/opencv/build"
cmake --build . --config Release
```

Linux/macOS:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Note: Ensure `OpenCV_DIR` (or pkg-config) points to an OpenCV build that
includes CUDA modules if you want to run GPU tests.

This CMake-based project will attempt to detect whether your OpenCV
installation contains the `cudaimgproc` module. If so, CMake defines
`USE_OPENCV_CUDA` and the code will compile the CUDA-backed path. When
CUDA modules are not present the project still builds: a CPU fallback
implementation is used for the GPU API so the benchmark runs correctly.

## Running

Default (uses `data/input`, `data/output`, `data/results`):

```bash
./opencv_test
```

Specify custom directories:

```bash
./opencv_test /path/to/input /path/to/output /path/to/results
```

Outputs:
- `data/output/edges_cpu_<filename>`: CPU edge image
- `data/output/edges_gpu_<filename>`: GPU edge image
- `data/results/benchmark_all.csv`: CSV summary appended with a row per image

## Benchmark Analysis

A generated analysis report with plots and summary statistics is available at [data/results/analysis.md](data/results/analysis.md). The report includes:

- CPU vs GPU runtime scatter (log-log)
- Speedup distribution histogram
- Runtime distribution boxplots
- PixelMatch vs EdgeIoU scatter

Open `data/results/analysis.md` to view the figures and detailed observations.

## Metrics

- Pixel match percentage: counts identical pixels between the two binary
  edge images and reports the percentage agreement.
- Edge IoU: computes intersection and union of edge pixels (bitwise AND/OR),
  returning 100 * intersection/union; if both images are empty, IoU is 100%.

## Notes & Troubleshooting

- If the GPU code fails due to missing CUDA modules, confirm that your
  OpenCV installation contains CUDA-enabled builds and that the CUDA toolkit
  and drivers are installed.
- The GPU timing reported includes upload and download overhead (host↔device).

## Continuous Integration

- A basic GitHub Actions workflow was added at
  [.github/workflows/ci.yml](.github/workflows/ci.yml) to configure and build
  the project on Ubuntu using the distro OpenCV packages (no CUDA).

## Files changed

- Updated source headers/impls with top-level documentation comments:
  - [src/canny_cpu.cpp](src/canny_cpu.cpp)
  - [src/canny_cpu.h](src/canny_cpu.h)
  - [src/canny_cuda.cpp](src/canny_cuda.cpp)
  - [src/canny_cuda.h](src/canny_cuda.h)
  - [src/image_io.cpp](src/image_io.cpp)
  - [src/image_io.h](src/image_io.h)
  - [src/main.cpp](src/main.cpp)
  - [src/metrics.cpp](src/metrics.cpp)
  - [src/metrics.h](src/metrics.h)
  - [src/timing.cpp](src/timing.cpp)
  - [src/timing.h](src/timing.h)

## License

See the project LICENSE file.

---
If you'd like, I can also:
- Add compile-time guards to gracefully fall back when OpenCV CUDA is not
  available.
- Add a simple CI job to build and run the benchmark on a sample image.
# GPU-Accelerated Canny Edge Detector

This project implements a Canny edge detector using **OpenCV CUDA** for GPU acceleration and standard CPU execution for comparison. It measures performance, computes quality metrics, and outputs both visual results and CSV benchmarks.

## Features

- Load images from a folder (`data/input`)
- Convert images to grayscale
- Run Canny edge detection on GPU (CUDA) and CPU
- Measure execution time and speedup
- Compute quality metrics:
  - **Pixel match percentage** between CPU and GPU results
  - **Edge IoU (Intersection over Union)** for edges
- Save output images and benchmark CSV to `data/output`
- Fully modularized C++ code with separate modules for:
  - GPU processing (`canny_cuda`)
  - CPU processing (`canny_cpu`)
  - Image I/O (`image_io`)
  - Metrics computation (`metrics`)
  - Timing utilities (`timing`)

## Project Structure





| Column         | Description                                      | Unit / Range                             |
| -------------- | ------------------------------------------------ | ---------------------------------------- |
| **Filename**   | Name of the input image                          | String                                   |
| **CPU_ms**     | Execution time of CPU-based Canny                | **milliseconds (ms)**                    |
| **GPU_ms**     | Execution time of GPU-based Canny                | **milliseconds (ms)**                    |
| **Speedup**    | How much faster GPU is than CPU                  | Dimensionless (ratio: `CPU_ms / GPU_ms`) |
| **PixelMatch** | Pixel-wise agreement between CPU and GPU results | Percentage (0–100%)                      |
| **EdgeIoU**    | Intersection over Union (IoU) of edges detected  | Percentage (0–100%)                      |
