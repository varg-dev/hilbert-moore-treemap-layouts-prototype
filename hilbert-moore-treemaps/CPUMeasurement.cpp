
#include "CPUMeasurement.h"

#include <iostream>
#include <chrono>


CPUMeasurement::CPUMeasurement(const std::string & identifier)
: m_identifier(identifier)
{
}

std::pair<float, float> CPUMeasurement::measure(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount)
{
    const auto duration = onMeasure(callback, warmupCount, executionCount);
    const auto durationPerIteration = duration / static_cast<float>(executionCount);

    return { duration, durationPerIteration };
}

void CPUMeasurement::measureAndPrint(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount)
{
    std::cerr << "Start Measurement of " << m_identifier << " (" << warmupCount << " warmup, " << executionCount << " measured iterations)" << std::endl;

    auto duration = 0.0f;
    auto durationPerIteration = 0.0f;

    std::tie(duration, durationPerIteration) = measure(callback, warmupCount, executionCount);

    // std::cerr << "Finished Measurement of " << m_identifier << std::endl;

    std::cerr << "Raw data: " << duration << std::endl;
    if (durationPerIteration < 1000) // µs
    {
        if (executionCount > 1)
        {
            std::cerr << duration << "µs for all iterations" << std::endl;
        }
        std::cerr << durationPerIteration << "µs for one iteration" << std::endl;
    }
    else if (durationPerIteration < 1000000) // ms
    {
        if (executionCount > 1)
        {
            std::cerr << duration / 1000 << "ms for all iterations" << std::endl;
        }
        std::cerr << durationPerIteration / 1000 << "ms for one iteration" << std::endl;
    }
    else // s
    {
        if (executionCount > 1)
        {
            std::cerr << duration / 1000000 << "s for all iterations" << std::endl;
        }
        std::cerr << durationPerIteration / 1000000 << "s for one iteration" << std::endl;
    }
}

float CPUMeasurement::onMeasure(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount)
{
    for (auto i = size_t(0); i < warmupCount; ++i)
    {
        callback();
    }

    const auto start = std::chrono::high_resolution_clock::now();

    for (auto i = size_t(0); i < executionCount; ++i)
    {
        callback();
    }

    const auto end = std::chrono::high_resolution_clock::now();
    return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}
