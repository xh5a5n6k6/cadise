#include "file-io/scene-description/valueParser.h"

#include "fundamental/logger/logger.h"

namespace cadise 
{

namespace
{
    const Logger logger("ValueParser");
}

bool ValueParser::parseBool(const std::string_view value)
{
    if (value == "true")
    {
        return true;
    }
    else if (value == "false")
    {
        return false;
    }

    logger.log("Failed to parse bool, unknown value: <" + std::string(value) + ">, return false instead.");
    return false;
}

std::string ValueParser::parseString(const std::string_view value)
{
    return std::string(value);
}

} // namespace cadise