#pragma once
#include <chrono>

/*
 * File: src/timing.h
 * Description: Declaration of `Timer`, a thin wrapper around
 * `std::chrono::high_resolution_clock` to measure elapsed time in
 * milliseconds.
 */

class Timer
{
public:
    void start();
    double stop(); // milliseconds

private:
    std::chrono::high_resolution_clock::time_point m_start;
};
