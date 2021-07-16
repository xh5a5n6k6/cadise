#include "fundamental/time/timePeriod.h"

#include <sstream>

namespace cadise {

TimePeriod::TimePeriod() :
    _sumMilliseconds(0) {
}

void TimePeriod::addMilliseconds(const uint64 milliseconds) {
    _sumMilliseconds += milliseconds;
}

void TimePeriod::reset() {
    _sumMilliseconds = 0;
}

std::string TimePeriod::toString() const {
    std::stringstream stringStream;

    if (_sumMilliseconds < 1000) {
        stringStream << _sumMilliseconds << "ms";
    }
    else {
        uint64 restMs;
        uint64 seconds;
        uint64 minutes;
        uint64 hours;

        seconds = _sumMilliseconds / 1000;
        restMs  = _sumMilliseconds % 1000;

        if (seconds >= 60) {
            minutes = seconds / 60;
            seconds = seconds % 60;

            if (minutes >= 60) {
                hours   = minutes / 60;
                minutes = minutes % 60;

                stringStream << hours << "h:";
            }

            stringStream << minutes << "m:";
        }

        stringStream << seconds + static_cast<real>(restMs) * 0.001_r << "s";
    }

    return stringStream.str();
}

} // namespace cadise