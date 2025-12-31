cmake_minimum_required(VERSION 3.20)
project(canny_edge_detector LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIR})

# If OpenCV was built with CUDA modules (e.g. cudaimgproc), enable
# a compile-time define so source can conditionally compile CUDA code.
string(TOLOWER "${OpenCV_LIBS}" __opencv_libs_lower)
if(__opencv_libs_lower MATCHES "cudaimgproc")
    message(STATUS "OpenCV CUDA modules detected; enabling USE_OPENCV_CUDA")
    add_compile_definitions(USE_OPENCV_CUDA)
endif()

# Ensure sample data directories exist so users can run the benchmark
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data/input ${CMAKE_SOURCE_DIR}/data/output ${CMAKE_SOURCE_DIR}/data/results)


add_executable(canny_edge_detector 
    src/main.cpp
    src/image_io.cpp
    src/canny_cpu.cpp
    src/canny_cuda.cpp
    src/metrics.cpp
    src/timing.cpp
)


target_link_libraries(canny_edge_detector PRIVATE ${OpenCV_LIBS})


