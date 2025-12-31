#include "canny_cpu.h"

/*
 * File: src/canny_cpu.cpp
 * Description: CPU implementation of the Canny edge detector using OpenCV.
 * Exposes: `cpu::runCanny(const cv::Mat&, double, double)` which returns
 * a binary edge image produced by `cv::Canny`.
 */

namespace cpu {

cv::Mat runCanny(
    const cv::Mat& gray,
    double lowThresh,
    double highThresh)
{
    cv::Mat edges;
    cv::Canny(gray, edges, lowThresh, highThresh);
    return edges;
}

}
