#pragma once

#include "math/fundamentalType.h"

#include <memory>
#include <string_view>

namespace cadise {

template<typename T>
class SdDataUnit {
public:
    SdDataUnit();
    SdDataUnit(const std::string_view& name, std::unique_ptr<T[]> value, const int32 valueNumber);

    std::string_view variableName() const;
    std::unique_ptr<T[]> value();
    int32 valueNumber() const;

private:
    std::string_view _variableName;
    std::unique_ptr<T[]> _value;
    int32 _valueNumber;
};

} // namespace cadise

#include "file-io/scene-description/sdDataUnit.ipp"