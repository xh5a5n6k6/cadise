#pragma once

#include <memory>
#include <string>

// forward declaration
namespace cadise
{
    class CSDResource;
}

namespace cadise
{

class CSDResourceParser
{
public:
    CSDResourceParser();

    std::shared_ptr<CSDResource> parse(const std::string& data) const;

private:
    void _parseDataUnit(
        const std::string& dataUnitString,
        CSDResource* const out_sdData) const;
};

} // namespace cadise