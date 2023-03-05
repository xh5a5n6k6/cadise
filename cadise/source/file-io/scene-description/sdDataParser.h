#pragma once

#include <memory>
#include <string>

// forward declaration
namespace cadise 
{ 
    class SdData; 
}

namespace cadise
{

class SdDataParser
{
public:
    SdDataParser();

    std::shared_ptr<SdData> parse(const std::string& data) const;

private:
    void _parseDataUnit(
        const std::string& dataUnitString,
        SdData* const      out_sdData) const;
};

} // namespace cadise