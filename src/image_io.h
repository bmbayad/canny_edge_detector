#pragma once
#include <opencv2/opencv.hpp>
#include <string>

/*
 * File: src/image_io.h
 * Description: Declarations for simple image I/O utilities used by the
 * benchmark runner: `io::loadGrayImage` and `io::saveImage`.
 */

namespace io {

cv::Mat loadGrayImage(const std::string& path);
void saveImage(const std::string& path, const cv::Mat& image);

}
