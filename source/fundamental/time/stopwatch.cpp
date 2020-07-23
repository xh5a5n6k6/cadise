#include "fundamental/time/stopwatch.h"

namespace cadise {

Stopwatch::Stopwatch() = default;

void Stopwatch::start() {
    _startTime = Clock::now();
}

void Stopwatch::stop() {
    const auto endTime  = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _startTime);

    _elapsedTime.addMilliseconds(static_cast<uint64>(duration.count()));
}

void Stopwatch::reset() {
    _elapsedTime.reset();
}

void Stopwatch::restart() {
    this->reset();
    this->start();
}

const TimePeriod& Stopwatch::elapsedTime() const {
    return _elapsedTime;
}

} // namespace cadise