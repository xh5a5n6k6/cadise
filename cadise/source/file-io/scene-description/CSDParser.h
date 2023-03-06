#pragma once

#include <memory>
#include <string>
#include <vector>

// forward declaration
namespace cadise 
{ 
    class CSDResource;
}

namespace cadise
{

/*! @brief Cadise scene description file parser.

It currently only supports scene file with .csda file extension.
*/
class CSDParser
{
public:
    static std::vector<std::shared_ptr<CSDResource>> parse(const std::string& filename);

private:
    static std::string _parseRaw(const std::string& filename);
};

} // namespace cadise