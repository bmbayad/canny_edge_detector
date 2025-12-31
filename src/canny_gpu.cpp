#include "canny_gpu.hpp"
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <iostream>

bool CannyGPU::isCudaAvailable() {
    return cv::cuda::getCudaEnabledDeviceCount() > 0;
}

std::string CannyGPU::getCudaDeviceInfo() {
    if (!isCudaAvailable()) {
        return "CUDA is not available";
    }
    
    int device_count = cv::cuda::getCudaEnabledDeviceCount();
    std::string info = "CUDA Devices: " + std::to_string(device_count) + "\n";
    
    for (int i = 0; i < device_count; i++) {
        cv::cuda::DeviceInfo dev_info(i);
        info += "Device " + std::to_string(i) + ": " + dev_info.name() + "\n";
        info += "  Compute Capability: " + std::to_string(dev_info.majorVersion()) + 
                "." + std::to_string(dev_info.minorVersion()) + "\n";
        info += "  Total Memory: " + std::to_string(dev_info.totalMemory() / (1024 * 1024)) + " MB\n";
    }
    
    return info;
}

cv::Mat CannyGPU::detect(const cv::Mat& input,
                         double threshold1,
                         double threshold2,
                         int aperture_size,
                         bool l2gradient) {
    if (!isCudaAvailable()) {
        std::cerr << "Error: CUDA is not available" << std::endl;
        return cv::Mat();
    }
    
    try {
        // Upload image to GPU
        cv::cuda::GpuMat d_input, d_gray, d_edges;
        d_input.upload(input);
        
        // Convert to grayscale if needed
        if (input.channels() == 3) {
            cv::cuda::cvtColor(d_input, d_gray, cv::COLOR_BGR2GRAY);
        } else {
            d_gray = d_input;
        }
        
        // Create Canny edge detector
        cv::Ptr<cv::cuda::CannyEdgeDetector> canny = 
            cv::cuda::createCannyEdgeDetector(threshold1, threshold2, aperture_size, l2gradient);
        
        // Apply Canny edge detection on GPU
        canny->detect(d_gray, d_edges);
        
        // Download result from GPU
        cv::Mat edges;
        d_edges.download(edges);
        
        return edges;
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV CUDA Error: " << e.what() << std::endl;
        return cv::Mat();
    }
}

bool CannyGPU::processImage(const std::string& input_path,
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
    
    // Apply Canny edge detection using GPU
    cv::Mat edges = detect(input, threshold1, threshold2, aperture_size, l2gradient);
    
    if (edges.empty()) {
        std::cerr << "Error: GPU edge detection failed" << std::endl;
        return false;
    }
    
    // Save output image
    if (!cv::imwrite(output_path, edges)) {
        std::cerr << "Error: Could not write image to " << output_path << std::endl;
        return false;
    }
    
    return true;
}
