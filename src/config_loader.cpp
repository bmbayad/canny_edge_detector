#include "config_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Config ConfigLoader::loadFromFile(const std::string& config_path) {
    Config config;
    std::ifstream file(config_path);
    
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file: " << config_path << std::endl;
        std::cerr << "Using default configuration." << std::endl;
        return createDefault();
    }
    
    std::string line;
    ImageConfig current_image;
    bool in_image_section = false;
    bool in_benchmark_section = false;
    
    // Set defaults
    config.benchmark.warmup_runs = 3;
    config.benchmark.benchmark_runs = 10;
    config.benchmark.save_outputs = true;
    
    while (std::getline(file, line)) {
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check for section headers
        if (line == "[benchmark]") {
            in_benchmark_section = true;
            in_image_section = false;
            continue;
        }
        
        if (line == "[image]") {
            // Save previous image config if exists
            if (in_image_section && !current_image.input_path.empty()) {
                config.images.push_back(current_image);
            }
            current_image = ImageConfig();
            current_image.threshold1 = 50.0;
            current_image.threshold2 = 150.0;
            current_image.aperture_size = 3;
            current_image.l2gradient = false;
            in_image_section = true;
            in_benchmark_section = false;
            continue;
        }
        
        // Parse key-value pairs
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            
            if (in_benchmark_section) {
                if (key == "warmup_runs") {
                    config.benchmark.warmup_runs = std::stoi(value);
                } else if (key == "benchmark_runs") {
                    config.benchmark.benchmark_runs = std::stoi(value);
                } else if (key == "save_outputs") {
                    config.benchmark.save_outputs = (value == "true" || value == "1");
                }
            } else if (in_image_section) {
                if (key == "input_path") {
                    current_image.input_path = value;
                } else if (key == "output_prefix") {
                    current_image.output_prefix = value;
                } else if (key == "threshold1") {
                    current_image.threshold1 = std::stod(value);
                } else if (key == "threshold2") {
                    current_image.threshold2 = std::stod(value);
                } else if (key == "aperture_size") {
                    current_image.aperture_size = std::stoi(value);
                } else if (key == "l2gradient") {
                    current_image.l2gradient = (value == "true" || value == "1");
                }
            }
        }
    }
    
    // Save last image config
    if (in_image_section && !current_image.input_path.empty()) {
        config.images.push_back(current_image);
    }
    
    file.close();
    
    // If no images were configured, use default
    if (config.images.empty()) {
        return createDefault();
    }
    
    return config;
}

Config ConfigLoader::createDefault() {
    Config config;
    
    ImageConfig img;
    img.input_path = "data/sample.jpg";
    img.output_prefix = "output";
    img.threshold1 = 50.0;
    img.threshold2 = 150.0;
    img.aperture_size = 3;
    img.l2gradient = false;
    
    config.images.push_back(img);
    config.benchmark.warmup_runs = 3;
    config.benchmark.benchmark_runs = 10;
    config.benchmark.save_outputs = true;
    
    return config;
}

std::string ConfigLoader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> ConfigLoader::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}
