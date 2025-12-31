#pragma once
#include <opencv2/opencv.hpp>

/*
 * File: src/metrics.h
 * Description: Declarations for image comparison metrics used in the
 * benchmark: `pixelMatchPercentage` and `edgeIoU`.
 */

namespace metrics {

double pixelMatchPercentage(const cv::Mat& a, const cv::Mat& b);
double edgeIoU(const cv::Mat& a, const cv::Mat& b);

}
