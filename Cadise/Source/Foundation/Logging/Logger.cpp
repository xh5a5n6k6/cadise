#include "Foundation/Logging/Logger.h"

#include "Foundation/Time/Timestamp.h"

#include <iostream>
#include <sstream>

namespace cadise
{

Logger::Logger(const std::string& senderName) :
    _senderName(senderName)
{}

void Logger::log(const std::string& message) const
{
    this->log(ELogLevel::Info, message);
}

void Logger::log(const ELogLevel logLevel, const std::string& message) const 
{
    const Timestamp timestamp;

    std::stringstream stringStream;
    stringStream << "[" << timestamp.toString() << "] ";

    switch (logLevel)
    {
        case ELogLevel::Trace:
            stringStream << "[Trace] ";
            break;

        case ELogLevel::Debug:
            stringStream << "[Debug] ";
            break;

        case ELogLevel::Info:
            stringStream << "[Info]  ";
            break;

        case ELogLevel::Warn:
            stringStream << "[Warn]  ";
            break;

        case ELogLevel::Error:
            stringStream << "[Error] ";
            break;

        default:
            stringStream << "Logger Error: unknown log level ";
            break;
    }

    stringStream << "[" << _senderName << "] " << message;

    std::cout << stringStream.str() << std::endl;
}

} // namespace cadise