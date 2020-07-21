#pragma once

#include <ctime>
#include <mutex>
#include <string>

namespace cadise {

class Timestamp {
public:
    Timestamp();

    std::string format() const;

private:
    std::time_t _currentTime;

    static std::mutex& _getMutex();
};

} // namespace cadise