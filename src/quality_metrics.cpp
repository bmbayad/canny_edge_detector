#include "quality_metrics.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

QualityMetrics QualityMetricsCalculator::calculate(const cv::Mat& img1, const cv::Mat& img2) {
    QualityMetrics metrics;
    
    metrics.psnr = calculatePSNR(img1, img2);
    metrics.ssim = calculateSSIM(img1, img2);
    metrics.pixel_accuracy = calculatePixelAccuracy(img1, img2);
    
    // Calculate matching pixels
    cv::Mat diff;
    cv::absdiff(img1, img2);
    cv::Mat mask = (diff == 0);
    metrics.matching_pixels = cv::countNonZero(mask);
    metrics.total_pixels = img1.rows * img1.cols;
    
    return metrics;
}

double QualityMetricsCalculator::calculatePSNR(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        std::cerr << "Error: Images must have the same size and type for PSNR calculation" << std::endl;
        return 0.0;
    }
    
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);
    
    cv::Scalar mse_scalar = cv::mean(diff);
    double mse = mse_scalar[0];
    
    if (mse <= 1e-10) {
        return 100.0; // Images are identical
    }
    
    double max_pixel = 255.0;
    double psnr = 10.0 * std::log10((max_pixel * max_pixel) / mse);
    
    return psnr;
}

double QualityMetricsCalculator::calculateSSIM(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        std::cerr << "Error: Images must have the same size and type for SSIM calculation" << std::endl;
        return 0.0;
    }
    
    const double C1 = 6.5025;  // (K1*L)^2, K1=0.01, L=255
    const double C2 = 58.5225; // (K2*L)^2, K2=0.03, L=255
    
    cv::Mat I1, I2;
    img1.convertTo(I1, CV_32F);
    img2.convertTo(I2, CV_32F);
    
    cv::Mat I1_2 = I1.mul(I1);
    cv::Mat I2_2 = I2.mul(I2);
    cv::Mat I1_I2 = I1.mul(I2);
    
    cv::Mat mu1, mu2;
    cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);
    
    cv::Mat mu1_2 = mu1.mul(mu1);
    cv::Mat mu2_2 = mu2.mul(mu2);
    cv::Mat mu1_mu2 = mu1.mul(mu2);
    
    cv::Mat sigma1_2, sigma2_2, sigma12;
    cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    
    cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    
    cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    
    cv::Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);
    
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);
    
    cv::Mat ssim_map;
    cv::divide(t3, t1, ssim_map);
    
    cv::Scalar mssim = cv::mean(ssim_map);
    return mssim[0];
}

double QualityMetricsCalculator::calculatePixelAccuracy(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        std::cerr << "Error: Images must have the same size and type for accuracy calculation" << std::endl;
        return 0.0;
    }
    
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    
    int matching = cv::countNonZero(diff == 0);
    int total = img1.rows * img1.cols * img1.channels();
    
    return (static_cast<double>(matching) / total) * 100.0;
}

void QualityMetricsCalculator::printMetrics(const QualityMetrics& metrics) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "QUALITY METRICS (CPU vs GPU)" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << std::left << std::setw(30) << "Metric" 
              << std::right << std::setw(20) << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << std::left << std::setw(30) << "PSNR (dB)" 
              << std::right << std::fixed << std::setprecision(2) 
              << std::setw(20) << metrics.psnr << std::endl;
    
    std::cout << std::left << std::setw(30) << "SSIM" 
              << std::right << std::fixed << std::setprecision(4) 
              << std::setw(20) << metrics.ssim << std::endl;
    
    std::cout << std::left << std::setw(30) << "Pixel Accuracy (%)" 
              << std::right << std::fixed << std::setprecision(2) 
              << std::setw(20) << metrics.pixel_accuracy << std::endl;
    
    std::cout << std::left << std::setw(30) << "Matching Pixels" 
              << std::right << std::setw(20) 
              << metrics.matching_pixels << " / " << metrics.total_pixels << std::endl;
    
    std::cout << std::string(60, '=') << std::endl;
}
