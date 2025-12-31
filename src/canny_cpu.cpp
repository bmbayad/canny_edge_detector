#include "canny_cpu.hpp"
#include <iostream>

cv::Mat CannyCPU::detect(const cv::Mat& input, 
                         double threshold1, 
                         double threshold2,
                         int aperture_size,
                         bool l2gradient) {
    cv::Mat edges;
    
    // Ensure input is grayscale
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Apply Canny edge detection
    cv::Canny(gray, edges, threshold1, threshold2, aperture_size, l2gradient);
    
    return edges;
}

bool CannyCPU::processImage(const std::string& input_path,
                            const std::string& output_path,
                            double threshold1,
                            double threshold2,
                            int aperture_size,
                            bool l2gradient) {
    // Read input image
    cv::Mat input = cv::imread(input_path);
    if (input.empty()) {
        std::cerr << "Error: Could not read image from " << input_path << std::endl;
        return false;
    }
    
    // Apply Canny edge detection
    cv::Mat edges = detect(input, threshold1, threshold2, aperture_size, l2gradient);
    
    // Save output image
    if (!cv::imwrite(output_path, edges)) {
        std::cerr << "Error: Could not write image to " << output_path << std::endl;
        return false;
    }
    
    return true;
}
