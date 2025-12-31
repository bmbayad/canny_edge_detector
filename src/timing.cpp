#include "timing.h"

/*
 * File: src/timing.cpp
 * Description: Simple high-resolution timer wrapper returning elapsed time
 * in milliseconds. Used by the benchmark to measure GPU/CPU runtimes.
 */

void Timer::start()
{
    m_start = std::chrono::high_resolution_clock::now();
}

double Timer::stop()
{
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - m_start).count();
}
