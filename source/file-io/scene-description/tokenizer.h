#pragma once

#include "math/type/mathType.h"

#include <memory>
#include <string_view>

namespace cadise {

class SdData;

class Tokenizer {
public:
    Tokenizer();

    std::shared_ptr<SdData> tokenize(const std::string_view& data) const;

private:
    void _parseDataUnit(std::shared_ptr<SdData>& sdData, const std::string_view& dataUnitString) const;

    // TODO: refactor here, tokenizer only do tokenization,
    //       use parser to do following functions
    std::unique_ptr<bool[]>             _parseBool(const std::string_view& value) const;
    std::unique_ptr<real[]>             _parseReal(const std::string_view& value) const;
    std::unique_ptr<int32[]>            _parseInt32(const std::string_view& value) const;
    std::unique_ptr<Vector3R[]>         _parseVector3r(const std::string_view& value) const;
    std::unique_ptr<std::string_view[]> _parseString(const std::string_view& value) const;

    std::unique_ptr<real[]>     _parseRealArray(
        const std::string_view& value, int32* const out_valueNumber) const;
    std::unique_ptr<Vector3R[]> _parseVector3rArray(
        const std::string_view& value, int32* const out_valueNumber) const;
};

} // namespace cadise