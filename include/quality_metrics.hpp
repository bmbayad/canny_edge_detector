#ifndef QUALITY_METRICS_HPP
#define QUALITY_METRICS_HPP

#include <opencv2/opencv.hpp>

struct QualityMetrics {
    double psnr;
    double ssim;
    double pixel_accuracy;
    int matching_pixels;
    int total_pixels;
};

class QualityMetricsCalculator {
public:
    /**
     * Calculate quality metrics between two images
     * @param img1 First image (reference)
     * @param img2 Second image (to compare)
     * @return Quality metrics
     */
    static QualityMetrics calculate(const cv::Mat& img1, const cv::Mat& img2);
    
    /**
     * Calculate Peak Signal-to-Noise Ratio
     * @param img1 First image
     * @param img2 Second image
     * @return PSNR value in dB
     */
    static double calculatePSNR(const cv::Mat& img1, const cv::Mat& img2);
    
    /**
     * Calculate Structural Similarity Index
     * @param img1 First image
     * @param img2 Second image
     * @return SSIM value (0-1)
     */
    static double calculateSSIM(const cv::Mat& img1, const cv::Mat& img2);
    
    /**
     * Calculate pixel accuracy (percentage of matching pixels)
     * @param img1 First image
     * @param img2 Second image
     * @return Accuracy as percentage (0-100)
     */
    static double calculatePixelAccuracy(const cv::Mat& img1, const cv::Mat& img2);
    
    /**
     * Print quality metrics in a formatted way
     * @param metrics Quality metrics to print
     */
    static void printMetrics(const QualityMetrics& metrics);
};

#endif // QUALITY_METRICS_HPP
