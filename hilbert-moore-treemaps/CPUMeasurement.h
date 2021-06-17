
#pragma once


#include <functional>
#include <string>
#include <utility>


class CPUMeasurement
{
public:
    CPUMeasurement(const std::string & identifier);

    void measureAndPrint(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount);
    std::pair<float, float> measure(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount);

protected:
    std::string m_identifier;
    bool m_initialized;

protected:
    float onMeasure(std::function<void()> callback, std::size_t warmupCount, std::size_t executionCount);
};
