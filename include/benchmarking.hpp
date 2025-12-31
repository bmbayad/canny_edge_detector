#ifndef BENCHMARKING_HPP
#define BENCHMARKING_HPP

#include <string>
#include <functional>
#include <vector>
#include <chrono>

struct BenchmarkResult {
    std::string name;
    double mean_time_ms;
    double min_time_ms;
    double max_time_ms;
    double std_dev_ms;
    int runs;
};

class Benchmarking {
public:
    /**
     * Benchmark a function with multiple runs
     * @param name Name of the benchmark
     * @param func Function to benchmark
     * @param warmup_runs Number of warmup runs
     * @param benchmark_runs Number of actual benchmark runs
     * @return Benchmark results
     */
    static BenchmarkResult benchmark(const std::string& name,
                                    std::function<void()> func,
                                    int warmup_runs = 3,
                                    int benchmark_runs = 10);
    
    /**
     * Print benchmark results in a formatted table
     * @param results Vector of benchmark results
     */
    static void printResults(const std::vector<BenchmarkResult>& results);
    
    /**
     * Calculate speedup between two benchmark results
     * @param baseline Baseline result (typically CPU)
     * @param optimized Optimized result (typically GPU)
     * @return Speedup factor
     */
    static double calculateSpeedup(const BenchmarkResult& baseline,
                                   const BenchmarkResult& optimized);
    
private:
    static double calculateMean(const std::vector<double>& values);
    static double calculateStdDev(const std::vector<double>& values, double mean);
};

#endif // BENCHMARKING_HPP
