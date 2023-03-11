#pragma once

#include "Foundation/Logging/ELogLevel.h"

#include <string>

namespace cadise 
{

class Logger
{
public:
    explicit Logger(const std::string& senderName);

    void log(const std::string& message) const;
    void log(const ELogLevel logLevel, const std::string& message) const;

private:
    std::string _senderName;
};

} // namespace cadise