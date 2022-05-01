#pragma once

#include "math/type/mathType.h"

#include <memory>
#include <string_view>

// forward declaration
namespace cadise 
{ 
    class SdData; 
} // end forward declaration

namespace cadise
{

class SdDataParser
{
public:
    SdDataParser();

    std::shared_ptr<SdData> parse(const std::string& data) const;

private:
    void _parseDataUnit(std::shared_ptr<SdData>& sdData, const std::string& dataUnitString) const;

    // TODO: refactor here, tokenizer only do tokenization,
    //       use parser to do following functions
    std::unique_ptr<bool[]>             _parseBool(const std::string& value) const;
    std::unique_ptr<real[]>             _parseReal(const std::string& value) const;
    std::unique_ptr<int32[]>            _parseInt32(const std::string& value) const;
    std::unique_ptr<Vector3R[]>         _parseVector3r(const std::string& value) const;
    std::unique_ptr<std::string[]> _parseString(const std::string& value) const;

    std::unique_ptr<real[]>     _parseRealArray(
        const std::string& value, 
        int32* const            out_valueNumber) const;

    std::unique_ptr<Vector3R[]> _parseVector3rArray(
        const std::string& value, 
        int32* const            out_valueNumber) const;
};

} // namespace cadise