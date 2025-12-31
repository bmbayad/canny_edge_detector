cmake_minimum_required(VERSION 3.18)
project(canny_edge_detector LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Find OpenCV with CUDA support
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs cudaimgproc cudawarping cudaarithm)

# Check if CUDA is available in OpenCV
if(NOT OpenCV_CUDA_VERSION)
    message(WARNING "OpenCV was not built with CUDA support. GPU acceleration will not be available.")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/canny_cpu.cpp
    src/canny_gpu.cpp
    src/benchmarking.cpp
    src/quality_metrics.cpp
    src/config_loader.cpp
)

# Header files
set(HEADERS
    include/canny_cpu.hpp
    include/canny_gpu.hpp
    include/benchmarking.hpp
    include/quality_metrics.hpp
    include/config_loader.hpp
)

# Create executable
add_executable(canny_edge_detector ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(canny_edge_detector
    ${OpenCV_LIBS}
)

# Set output directory
set_target_properties(canny_edge_detector
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable warnings
if(MSVC)
    target_compile_options(canny_edge_detector PRIVATE /W4)
else()
    target_compile_options(canny_edge_detector PRIVATE -Wall -Wextra -pedantic)
endif()

# Print configuration
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV CUDA version: ${OpenCV_CUDA_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
