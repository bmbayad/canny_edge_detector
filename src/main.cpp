#include <iostream>
#include <fstream>
#include <filesystem>

#include "image_io.h"
#include "canny_cpu.h"
#include "canny_cuda.h"
#include "metrics.h"
#include "timing.h"

/*
 * File: src/main.cpp
 * Description: Benchmark runner that iterates over images in
 * `data/input`, runs the GPU and CPU Canny detectors, measures timing,
 * computes comparison metrics, writes output images to `data/output`,
 * and appends benchmark rows to `data/results/benchmark_all.csv`.
 *
 * Usage: `main [input_dir] [output_dir] [results_dir]` (all optional)
 */

int main(int argc, char **argv)
{
    try
    {
        constexpr double LOW = 50.0;
        constexpr double HIGH = 150.0;

        namespace fs = std::filesystem;

        // Determine input and output directories
        fs::path inputDir = (argc > 1) ? fs::path(argv[1]) : fs::current_path() / "data" / "input";
        fs::path outputDir = (argc > 2) ? fs::path(argv[2]) : fs::current_path() / "data" / "output";
        fs::path results = (argc > 3) ? fs::path(argv[3]) : fs::current_path() / "data" / "results";

        //check if input directory exists
        if (!fs::exists(inputDir) || !fs::is_directory(inputDir))
        {
            throw std::runtime_error("Input directory does not exist: " + inputDir.string());
        }


        // create output and results directories if they don't exist
        fs::create_directories(outputDir);
        fs::create_directories(results);

        // Print directories
        std::cout << "Input directory: " << inputDir.string() << "\n";
        std::cout << "Output directory: " << outputDir.string() << "\n";
        std::cout << "Results directory: " << results.string() << "\n";

        // csv file to store benchmark for all images
        std::ofstream csv(results / "benchmark_all.csv");
        csv << "Filename,CPU_ms,GPU_ms,Speedup,PixelMatch,EdgeIoU\n";

        Timer timer;

        for (const auto &entry : fs::directory_iterator(inputDir))
        {
            if (!entry.is_regular_file())
                continue;

            fs::path inputImage = entry.path();
            std::string filename = inputImage.filename().string();

            std::cout << "Processing: " << filename << "\n";

            cv::Mat gray = io::loadGrayImage(inputImage.string());

            // Run CPU Canny (always available)
            timer.start();
            cv::Mat cpuEdges = cpu::runCanny(gray, LOW, HIGH);
            double cpuTime = timer.stop();

            cv::Mat gpuEdges;
            double gpuTime = 0.0;

            if (gpu::isCudaAvailable())
            {
                timer.start();
                gpuEdges = gpu::runCanny(gray, LOW, HIGH);
                gpuTime = timer.stop();
            }
            else
            {
                // GPU not available; use CPU result as fallback (so comparisons
                // remain valid) and record GPU time as 0.0
                std::cout << "GPU not available — using CPU result as fallback.\n";
                gpuEdges = cpuEdges;
            }

            double pixelMatch = metrics::pixelMatchPercentage(cpuEdges, gpuEdges);
            double edgeIoU = metrics::edgeIoU(cpuEdges, gpuEdges);

            // Save outputs
            io::saveImage((outputDir / ("edges_cpu_" + filename)).string(), cpuEdges);
            io::saveImage((outputDir / ("edges_gpu_" + filename)).string(), gpuEdges);

            // Compute speedup (if GPU time is zero, speedup is 1.0)
            double speedup = (gpuTime > 0.0) ? (cpuTime / gpuTime) : 1.0;

            // Save benchmark
            csv << filename << ","
                << cpuTime << ","
                << gpuTime << ","
                << speedup << ","
                << pixelMatch << ","
                << edgeIoU << "\n";

            std::cout << "CPU time: " << cpuTime << " ms | "
                      << "GPU time: " << gpuTime << " ms | "
                      << "Speedup: " << speedup << "x | "
                      << "Pixel match: " << pixelMatch << "% | "
                      << "Edge IoU: " << edgeIoU << "%\n\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}