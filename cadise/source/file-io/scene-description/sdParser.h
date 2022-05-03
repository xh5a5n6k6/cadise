#pragma once

#include <memory>
#include <string>
#include <vector>

// forward declaration
namespace cadise 
{ 
    class SdData;
    class RenderDatabase;
    class Tokenizer; 
}

namespace cadise
{

/*! @brief Scene description file (with .crsd extension) parser.
*/
class SdParser
{
public:
    void parse(
        const std::string& filename,
        RenderDatabase* ren,
        std::vector<std::shared_ptr<SdData>>& out_sdDatas) const;

private:
    std::string _parseRaw(const std::string& filename) const;
};

} // namespace cadise