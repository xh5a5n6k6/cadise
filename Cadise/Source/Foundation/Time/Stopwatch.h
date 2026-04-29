#pragma once

#include "Foundation/Time/TimePeriod.h"

#include <chrono>

namespace cadise
{

class Stopwatch
{
private:
    using Clock = std::chrono::steady_clock;

public:
    Stopwatch();

    void start();
    void stop();
    void reset();
    void restart();

    const TimePeriod& elapsedTime() const;

private:
    Clock::time_point _startTime;
    TimePeriod        _elapsedTime;
};

} // namespace cadise