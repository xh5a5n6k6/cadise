#pragma once

#include "math/vector.h"

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

    std::unique_ptr<real[]>             _parseReal(const std::string_view& value) const;
    std::unique_ptr<int32[]>            _parseInt32(const std::string_view& value) const;
    std::unique_ptr<Vector3R[]>         _parseVector3R(const std::string_view& value) const;
    std::unique_ptr<std::string_view[]> _parseString(const std::string_view& value) const;

    std::unique_ptr<real[]>     _parseRealArray(
        const std::string_view& value, int32* const valueNumber) const;
    std::unique_ptr<Vector3R[]> _parseVector3RArray(
        const std::string_view& value, int32* const valueNumber) const;
};

} // namespace cadise