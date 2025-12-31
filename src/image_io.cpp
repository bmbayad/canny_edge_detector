#include "image_io.h"
#include <stdexcept>

/*
 * File: src/image_io.cpp
 * Description: Simple image I/O helpers for the benchmark. Provides
 * `io::loadGrayImage` to read a grayscale image and `io::saveImage` to write
 * images to disk. These functions throw `std::runtime_error` on failure.
 */

namespace io {

cv::Mat loadGrayImage(const std::string& path)
{
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (img.empty())
        throw std::runtime_error("Failed to load image: " + path);
    return img;
}

void saveImage(const std::string& path, const cv::Mat& image)
{
    if (!cv::imwrite(path, image))
        throw std::runtime_error("Failed to save image: " + path);
}

}
