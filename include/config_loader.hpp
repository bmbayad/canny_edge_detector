#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>
#include <vector>

struct ImageConfig {
    std::string input_path;
    std::string output_prefix;
    double threshold1;
    double threshold2;
    int aperture_size;
    bool l2gradient;
};

struct BenchmarkConfig {
    int warmup_runs;
    int benchmark_runs;
    bool save_outputs;
};

struct Config {
    std::vector<ImageConfig> images;
    BenchmarkConfig benchmark;
};

class ConfigLoader {
public:
    static Config loadFromFile(const std::string& config_path);
    static Config createDefault();
    
private:
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);
};

#endif // CONFIG_LOADER_HPP
