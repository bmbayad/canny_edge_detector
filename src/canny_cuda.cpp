#include "canny_cuda.h"

/*
 * File: src/canny_cuda.cpp
 * Description: GPU-accelerated Canny edge detector using OpenCV CUDA when
 * available. When compiled without CUDA support, a CPU fallback is used so
 * that the project still builds and runs (GPU calls forward to CPU Canny).
 */

#ifdef USE_OPENCV_CUDA
#include <opencv2/cudaimgproc.hpp>

namespace gpu {

cv::Mat runCanny(
    const cv::Mat& gray,
    double lowThresh,
    double highThresh)
{
    cv::cuda::GpuMat d_gray, d_edges;
    d_gray.upload(gray);

    auto canny = cv::cuda::createCannyEdgeDetector(lowThresh, highThresh);
    canny->detect(d_gray, d_edges);

    cv::Mat edges;
    d_edges.download(edges);
    return edges;
}

bool isCudaAvailable()
{
    try {
        return cv::cuda::getCudaEnabledDeviceCount() > 0;
    } catch (...) {
        return false;
    }
}

} // namespace gpu

#else // USE_OPENCV_CUDA

#include "canny_cpu.h"

namespace gpu {

cv::Mat runCanny(
    const cv::Mat& gray,
    double lowThresh,
    double highThresh)
{
    // Fallback: run CPU implementation and return its result.
    return cpu::runCanny(gray, lowThresh, highThresh);
}

bool isCudaAvailable()
{
    return false;
}

} // namespace gpu

#endif // USE_OPENCV_CUDA
