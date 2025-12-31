# Building the Project

This guide provides detailed instructions for building the Canny Edge Detector project.

## Prerequisites

Before building, ensure you have all required dependencies installed:

### Required Software

1. **CMake** (version 3.18 or higher)
   ```bash
   cmake --version
   ```

2. **C++ Compiler** with C++17 support
   - GCC 7+ or Clang 5+ on Linux
   - Visual Studio 2017+ on Windows
   - Xcode 10+ on macOS

3. **CUDA Toolkit** (version 10.2 or higher)
   ```bash
   nvcc --version
   ```

4. **OpenCV** with CUDA support (version 4.x)
   ```bash
   pkg-config --modversion opencv4
   ```

## Quick Build

For a quick build, use the provided build script:

```bash
./build.sh
```

This script will:
1. Remove any existing build directory
2. Create a new build directory
3. Run CMake configuration
4. Build the project using all available CPU cores

## Manual Build

For more control over the build process:

### Linux / macOS

```bash
# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Optional: Configure with specific options
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      ..

# Build
make -j$(nproc)

# Optional: Install (requires sudo)
sudo make install
```

### Windows (Visual Studio)

```batch
# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake -G "Visual Studio 16 2019" -A x64 ..

# Build using CMake
cmake --build . --config Release

# Or open the solution in Visual Studio
start canny_edge_detector.sln
```

## Build Configurations

### Debug Build

```bash
mkdir -p build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Release Build with Optimizations

```bash
mkdir -p build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS="-O3 -march=native" \
      ..
make -j$(nproc)
```

## CMake Options

### Specifying OpenCV Location

If CMake cannot find OpenCV:

```bash
cmake -DOpenCV_DIR=/path/to/opencv/build ..
```

### CUDA Architecture

To specify CUDA compute capability:

```bash
cmake -DCUDA_ARCH_BIN="7.5" ..
```

Common compute capabilities:
- 6.1: GTX 1080, GTX 1070, GTX 1060
- 7.0: Tesla V100
- 7.5: RTX 2080 Ti, RTX 2080, RTX 2070
- 8.0: A100
- 8.6: RTX 3090, RTX 3080, RTX 3070

## Verifying the Build

After building, verify the executable:

```bash
# Check if executable exists
ls -lh build/bin/canny_edge_detector

# Run help to verify it works
./build/bin/canny_edge_detector --help

# Check CUDA availability
./build/bin/canny_edge_detector | grep -i cuda
```

## Troubleshooting Build Issues

### OpenCV Not Found

```
CMake Error: Could not find OpenCV
```

**Solution**: Set the OpenCV_DIR variable:
```bash
export OpenCV_DIR=/usr/local/lib/cmake/opencv4
cmake ..
```

Or specify during configuration:
```bash
cmake -DOpenCV_DIR=/usr/local/lib/cmake/opencv4 ..
```

### CUDA Not Found

```
CMake Error: Could not find CUDA
```

**Solution**: Add CUDA to your PATH:
```bash
export PATH=/usr/local/cuda/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

### OpenCV Without CUDA Support

```
Warning: OpenCV was not built with CUDA support
```

**Solution**: Rebuild OpenCV with CUDA enabled. See main README for instructions.

### Compilation Errors

If you encounter C++ compilation errors:

1. Check C++ standard support:
   ```bash
   g++ --version  # Should be 7.0 or higher
   ```

2. Explicitly set C++ standard:
   ```bash
   cmake -DCMAKE_CXX_STANDARD=17 ..
   ```

### Linking Errors

If you encounter linking errors with OpenCV:

1. Check OpenCV installation:
   ```bash
   pkg-config --libs opencv4
   ```

2. Update library cache (Linux):
   ```bash
   sudo ldconfig
   ```

## Clean Build

To perform a clean build:

```bash
# Remove build directory
rm -rf build

# Rebuild
./build.sh
```

## Build Output

After a successful build, you should see:

```
build/
├── bin/
│   └── canny_edge_detector    # Main executable
├── CMakeFiles/                 # CMake build files
└── Makefile                    # Generated Makefile
```

## Next Steps

After building successfully:

1. **Run the test**: `./test.sh`
2. **Try with your images**: `./build/bin/canny_edge_detector -i your_image.jpg`
3. **Read usage guide**: See README.md for detailed usage instructions

## Performance Build Notes

For maximum performance:

1. Use Release build type
2. Enable CPU optimizations (`-march=native`)
3. Ensure GPU drivers are up to date
4. Build OpenCV with same CUDA version
5. Match CUDA architecture to your GPU

Example performance build:

```bash
mkdir -p build-performance
cd build-performance

cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG" \
      -DCUDA_ARCH_BIN="8.6" \
      ..

make -j$(nproc)
```
