#pragma once
#include <opencv2/opencv.hpp>

/*
 * File: src/canny_cpu.h
 * Description: Header for the CPU Canny wrapper. Declares `cpu::runCanny`.
 */

namespace cpu {

cv::Mat runCanny(
    const cv::Mat& gray,
    double lowThresh,
    double highThresh
);

}
