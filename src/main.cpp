#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "config_loader.hpp"
#include "canny_cpu.hpp"
#include "canny_gpu.hpp"
#include "benchmarking.hpp"
#include "quality_metrics.hpp"

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]" << std::endl;
    std::cout << "\nOptions:" << std::endl;
    std::cout << "  -c, --config <file>     Path to configuration file" << std::endl;
    std::cout << "  -i, --input <file>      Input image file" << std::endl;
    std::cout << "  -o, --output <prefix>   Output file prefix (default: output)" << std::endl;
    std::cout << "  -t1 <value>             Canny threshold 1 (default: 50)" << std::endl;
    std::cout << "  -t2 <value>             Canny threshold 2 (default: 150)" << std::endl;
    std::cout << "  -a, --aperture <size>   Aperture size (3, 5, or 7; default: 3)" << std::endl;
    std::cout << "  -l2                     Use L2 gradient" << std::endl;
    std::cout << "  --cpu-only              Run CPU version only" << std::endl;
    std::cout << "  --gpu-only              Run GPU version only" << std::endl;
    std::cout << "  --no-benchmark          Skip benchmarking" << std::endl;
    std::cout << "  --no-save               Don't save output images" << std::endl;
    std::cout << "  -h, --help              Show this help message" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  " << program_name << " -c config.txt" << std::endl;
    std::cout << "  " << program_name << " -i image.jpg -t1 50 -t2 150" << std::endl;
    std::cout << "  " << program_name << " -i image.jpg --gpu-only" << std::endl;
}

void printHeader() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "  GPU CANNY EDGE DETECTION - OpenCV CUDA Demo" << std::endl;
    std::cout << std::string(80, '=') << std::endl << std::endl;
}

void processWithBenchmark(const ImageConfig& img_config, 
                         bool run_cpu, 
                         bool run_gpu, 
                         bool do_benchmark,
                         bool save_outputs,
                         int warmup_runs,
                         int benchmark_runs) {
    
    std::cout << "\nProcessing: " << img_config.input_path << std::endl;
    std::cout << "Parameters: threshold1=" << img_config.threshold1 
              << ", threshold2=" << img_config.threshold2
              << ", aperture=" << img_config.aperture_size
              << ", L2gradient=" << (img_config.l2gradient ? "true" : "false") << std::endl;
    
    // Load input image
    cv::Mat input = cv::imread(img_config.input_path);
    if (input.empty()) {
        std::cerr << "Error: Could not load image: " << img_config.input_path << std::endl;
        return;
    }
    
    std::cout << "Image size: " << input.cols << "x" << input.rows << std::endl;
    
    cv::Mat cpu_result, gpu_result;
    std::vector<BenchmarkResult> results;
    
    // CPU processing
    if (run_cpu) {
        std::cout << "\nRunning CPU Canny Edge Detection..." << std::endl;
        
        if (do_benchmark) {
            auto cpu_benchmark = Benchmarking::benchmark(
                "CPU Canny",
                [&]() {
                    cpu_result = CannyCPU::detect(input, 
                                                  img_config.threshold1, 
                                                  img_config.threshold2,
                                                  img_config.aperture_size,
                                                  img_config.l2gradient);
                },
                warmup_runs,
                benchmark_runs
            );
            results.push_back(cpu_benchmark);
        } else {
            cpu_result = CannyCPU::detect(input, 
                                         img_config.threshold1, 
                                         img_config.threshold2,
                                         img_config.aperture_size,
                                         img_config.l2gradient);
        }
        
        if (save_outputs && !cpu_result.empty()) {
            std::string cpu_output = img_config.output_prefix + "_cpu.png";
            cv::imwrite(cpu_output, cpu_result);
            std::cout << "CPU result saved to: " << cpu_output << std::endl;
        }
    }
    
    // GPU processing
    if (run_gpu) {
        if (!CannyGPU::isCudaAvailable()) {
            std::cerr << "\nError: CUDA is not available. GPU processing skipped." << std::endl;
            std::cout << "Please ensure OpenCV is built with CUDA support." << std::endl;
        } else {
            std::cout << "\nRunning GPU Canny Edge Detection..." << std::endl;
            
            if (do_benchmark) {
                auto gpu_benchmark = Benchmarking::benchmark(
                    "GPU Canny (CUDA)",
                    [&]() {
                        gpu_result = CannyGPU::detect(input, 
                                                      img_config.threshold1, 
                                                      img_config.threshold2,
                                                      img_config.aperture_size,
                                                      img_config.l2gradient);
                    },
                    warmup_runs,
                    benchmark_runs
                );
                results.push_back(gpu_benchmark);
            } else {
                gpu_result = CannyGPU::detect(input, 
                                             img_config.threshold1, 
                                             img_config.threshold2,
                                             img_config.aperture_size,
                                             img_config.l2gradient);
            }
            
            if (save_outputs && !gpu_result.empty()) {
                std::string gpu_output = img_config.output_prefix + "_gpu.png";
                cv::imwrite(gpu_output, gpu_result);
                std::cout << "GPU result saved to: " << gpu_output << std::endl;
            }
        }
    }
    
    // Print benchmark results
    if (do_benchmark && !results.empty()) {
        Benchmarking::printResults(results);
    }
    
    // Calculate quality metrics if both CPU and GPU results exist
    if (!cpu_result.empty() && !gpu_result.empty()) {
        QualityMetrics metrics = QualityMetricsCalculator::calculate(cpu_result, gpu_result);
        QualityMetricsCalculator::printMetrics(metrics);
    }
}

int main(int argc, char* argv[]) {
    printHeader();
    
    // Parse command line arguments
    std::string config_file;
    std::string input_file;
    std::string output_prefix = "output";
    double threshold1 = 50.0;
    double threshold2 = 150.0;
    int aperture_size = 3;
    bool l2gradient = false;
    bool run_cpu = true;
    bool run_gpu = true;
    bool do_benchmark = true;
    bool save_outputs = true;
    int warmup_runs = 3;
    int benchmark_runs = 10;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                config_file = argv[++i];
            }
        } else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                input_file = argv[++i];
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                output_prefix = argv[++i];
            }
        } else if (arg == "-t1") {
            if (i + 1 < argc) {
                threshold1 = std::stod(argv[++i]);
            }
        } else if (arg == "-t2") {
            if (i + 1 < argc) {
                threshold2 = std::stod(argv[++i]);
            }
        } else if (arg == "-a" || arg == "--aperture") {
            if (i + 1 < argc) {
                aperture_size = std::stoi(argv[++i]);
            }
        } else if (arg == "-l2") {
            l2gradient = true;
        } else if (arg == "--cpu-only") {
            run_gpu = false;
        } else if (arg == "--gpu-only") {
            run_cpu = false;
        } else if (arg == "--no-benchmark") {
            do_benchmark = false;
        } else if (arg == "--no-save") {
            save_outputs = false;
        }
    }
    
    // Print CUDA device information
    if (CannyGPU::isCudaAvailable()) {
        std::cout << CannyGPU::getCudaDeviceInfo() << std::endl;
    } else {
        std::cout << "CUDA Status: Not available" << std::endl;
        std::cout << "Note: GPU processing will be skipped." << std::endl;
        std::cout << "      Please ensure OpenCV is built with CUDA support.\n" << std::endl;
    }
    
    // Load configuration or use command line parameters
    Config config;
    
    if (!config_file.empty()) {
        std::cout << "Loading configuration from: " << config_file << std::endl;
        config = ConfigLoader::loadFromFile(config_file);
    } else if (!input_file.empty()) {
        // Use command line parameters
        ImageConfig img_config;
        img_config.input_path = input_file;
        img_config.output_prefix = output_prefix;
        img_config.threshold1 = threshold1;
        img_config.threshold2 = threshold2;
        img_config.aperture_size = aperture_size;
        img_config.l2gradient = l2gradient;
        
        config.images.push_back(img_config);
        config.benchmark.warmup_runs = warmup_runs;
        config.benchmark.benchmark_runs = benchmark_runs;
        config.benchmark.save_outputs = save_outputs;
    } else {
        std::cout << "No configuration or input file specified." << std::endl;
        std::cout << "Using default configuration..." << std::endl;
        config = ConfigLoader::createDefault();
    }
    
    // Process each image
    for (const auto& img_config : config.images) {
        processWithBenchmark(img_config, 
                           run_cpu, 
                           run_gpu, 
                           do_benchmark,
                           save_outputs,
                           config.benchmark.warmup_runs,
                           config.benchmark.benchmark_runs);
    }
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "Processing complete!" << std::endl;
    std::cout << std::string(80, '=') << std::endl << std::endl;
    
    return 0;
}
