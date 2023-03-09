#include "fundamental/time/timestamp.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace cadise 
{

Timestamp::Timestamp() 
{
    const auto timePoint = std::chrono::system_clock::now();

    _currentTime = std::chrono::system_clock::to_time_t(timePoint);
}

std::string Timestamp::toString() const 
{
    std::lock_guard<std::mutex> lock(_getMutex());

    std::stringstream stringStream;
    stringStream << std::put_time(std::localtime(&_currentTime), "%Y-%m-%d %X");

    return stringStream.str();
}

std::mutex& Timestamp::_getMutex()
{
    static std::mutex mutex;

    return mutex;
}

} // namespace cadise