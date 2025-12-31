#ifndef CANNY_CPU_HPP
#define CANNY_CPU_HPP

#include <opencv2/opencv.hpp>
#include <string>

class CannyCPU {
public:
    /**
     * Apply Canny edge detection using CPU
     * @param input Input grayscale image
     * @param threshold1 First threshold for the hysteresis procedure
     * @param threshold2 Second threshold for the hysteresis procedure
     * @param aperture_size Aperture size for Sobel operator
     * @param l2gradient Flag indicating whether to use L2 norm for gradient magnitude
     * @return Edge-detected image
     */
    static cv::Mat detect(const cv::Mat& input, 
                         double threshold1, 
                         double threshold2,
                         int aperture_size = 3,
                         bool l2gradient = false);
    
    /**
     * Process an image file and save the result
     * @param input_path Path to input image
     * @param output_path Path to save output image
     * @param threshold1 First threshold
     * @param threshold2 Second threshold
     * @param aperture_size Aperture size for Sobel operator
     * @param l2gradient Flag for L2 gradient
     * @return True if successful, false otherwise
     */
    static bool processImage(const std::string& input_path,
                            const std::string& output_path,
                            double threshold1,
                            double threshold2,
                            int aperture_size = 3,
                            bool l2gradient = false);
};

#endif // CANNY_CPU_HPP
