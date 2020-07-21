#include "fundamental/logger/logger.h"

#include "fundamental/time/timestamp.h"

#include <iostream>
#include <sstream>

namespace cadise {

Logger::Logger(const std::string& senderName) :
    _senderName(senderName) {
}

void Logger::log(const std::string& message) const {
    this->log(ELogLevel::INFO, message);
}

void Logger::log(const ELogLevel& logLevel, const std::string& message) const {
    const Timestamp timestamp;

    std::stringstream stringStream;
    stringStream << "[" << timestamp.format() << "] ";

    switch (logLevel) {
        case ELogLevel::INFO:
            stringStream << "[INFO]  ";
            break;

        case ELogLevel::WARN:
            stringStream << "[WARN]  ";
            break;

        case ELogLevel::ERROR:
            stringStream << "[ERROR] ";
            break;

        default:
            stringStream << "Logger Error: unknown log level ";
            break;
    }

    stringStream << "[" << _senderName << "] " << message;

    std::cerr << stringStream.str() << std::endl;
}

} // namespace cadise