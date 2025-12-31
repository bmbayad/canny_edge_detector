#include "metrics.h"

/*
 * File: src/metrics.cpp
 * Description: Implements comparison metrics between two edge images.
 * - `pixelMatchPercentage` returns the percentage of identical pixels
 *   between two same-sized binary images.
 * - `edgeIoU` computes Intersection-over-Union for edge pixels.
 */

namespace metrics {

double pixelMatchPercentage(const cv::Mat& a, const cv::Mat& b)
{
    CV_Assert(a.size() == b.size());
    CV_Assert(a.type() == b.type());

    cv::Mat diff;
    cv::absdiff(a, b, diff);

    int mismatches = cv::countNonZero(diff);
    int total = a.rows * a.cols;

    return 100.0 * (total - mismatches) / total;
}

double edgeIoU(const cv::Mat& a, const cv::Mat& b)
{
    cv::Mat inter, uni;
    cv::bitwise_and(a, b, inter);
    cv::bitwise_or(a, b, uni);

    int unionCount = cv::countNonZero(uni);
    if (unionCount == 0)
        return 100.0;

    int intersection = cv::countNonZero(inter);
    return 100.0 * intersection / unionCount;
}

}
