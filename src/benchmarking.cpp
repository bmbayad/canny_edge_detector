#include "benchmarking.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <algorithm>

BenchmarkResult Benchmarking::benchmark(const std::string& name,
                                       std::function<void()> func,
                                       int warmup_runs,
                                       int benchmark_runs) {
    BenchmarkResult result;
    result.name = name;
    result.runs = benchmark_runs;
    
    // Warmup runs
    for (int i = 0; i < warmup_runs; i++) {
        func();
    }
    
    // Benchmark runs
    std::vector<double> times;
    times.reserve(benchmark_runs);
    
    for (int i = 0; i < benchmark_runs; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        times.push_back(elapsed);
    }
    
    // Calculate statistics
    result.mean_time_ms = calculateMean(times);
    result.std_dev_ms = calculateStdDev(times, result.mean_time_ms);
    result.min_time_ms = *std::min_element(times.begin(), times.end());
    result.max_time_ms = *std::max_element(times.begin(), times.end());
    
    return result;
}

void Benchmarking::printResults(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "BENCHMARK RESULTS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << std::left << std::setw(25) << "Name"
              << std::right << std::setw(12) << "Mean (ms)"
              << std::setw(12) << "Min (ms)"
              << std::setw(12) << "Max (ms)"
              << std::setw(12) << "StdDev (ms)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& result : results) {
        std::cout << std::left << std::setw(25) << result.name
                  << std::right << std::fixed << std::setprecision(3)
                  << std::setw(12) << result.mean_time_ms
                  << std::setw(12) << result.min_time_ms
                  << std::setw(12) << result.max_time_ms
                  << std::setw(12) << result.std_dev_ms << std::endl;
    }
    
    std::cout << std::string(80, '=') << std::endl;
    
    // Calculate and print speedups if we have both CPU and GPU results
    if (results.size() >= 2) {
        std::cout << "\nSPEEDUP ANALYSIS" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (size_t i = 1; i < results.size(); i++) {
            if (results[i].name.find("GPU") != std::string::npos && 
                results[0].name.find("CPU") != std::string::npos) {
                double speedup = calculateSpeedup(results[0], results[i]);
                std::cout << results[i].name << " vs " << results[0].name 
                          << ": " << std::fixed << std::setprecision(2) 
                          << speedup << "x speedup" << std::endl;
            }
        }
        std::cout << std::string(80, '=') << std::endl;
    }
}

double Benchmarking::calculateSpeedup(const BenchmarkResult& baseline,
                                     const BenchmarkResult& optimized) {
    if (optimized.mean_time_ms == 0) {
        return 0.0;
    }
    return baseline.mean_time_ms / optimized.mean_time_ms;
}

double Benchmarking::calculateMean(const std::vector<double>& values) {
    if (values.empty()) {
        return 0.0;
    }
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double Benchmarking::calculateStdDev(const std::vector<double>& values, double mean) {
    if (values.empty()) {
        return 0.0;
    }
    
    double variance = 0.0;
    for (double value : values) {
        variance += (value - mean) * (value - mean);
    }
    variance /= values.size();
    
    return std::sqrt(variance);
}
