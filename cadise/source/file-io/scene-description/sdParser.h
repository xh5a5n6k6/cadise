#pragma once

#include <memory>
#include <string>
#include <vector>

// forward declaration
namespace cadise 
{ 
    class SdData;
}

namespace cadise
{

/*! @brief Scene description file (with .crsd extension) parser.
*/
class SdParser
{
public:
    static std::vector<std::shared_ptr<SdData>> parse(const std::string& filename);

private:
    static std::string _parseRaw(const std::string& filename);
};

} // namespace cadise