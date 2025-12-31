#pragma once
#include <opencv2/core.hpp>

/*
 * File: src/canny_cuda.h
 * Description: Header for GPU Canny wrapper. Declares `gpu::runCanny` and
 * `gpu::isCudaAvailable`.
 * When compiled with `USE_OPENCV_CUDA` (set by CMake if `cudaimgproc` is
 * available), the implementation uses OpenCV CUDA APIs. Otherwise a CPU
 * fallback implementation is provided so the project can build on systems
 * without CUDA-enabled OpenCV.
 */

namespace gpu {

cv::Mat runCanny(
    const cv::Mat& gray,
    double lowThresh,
    double highThresh
);

// Returns true if a CUDA-enabled OpenCV implementation is available at
// runtime (i.e., code was built with CUDA support and a device is present).
bool isCudaAvailable();

}
