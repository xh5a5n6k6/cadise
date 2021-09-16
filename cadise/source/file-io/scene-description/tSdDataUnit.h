#pragma once

#include "math/type/fundamentalType.h"

#include <memory>
#include <string_view>

namespace cadise 
{

template<typename T>
class TSdDataUnit 
{
public:
    TSdDataUnit();
    TSdDataUnit(const std::string_view& name, std::unique_ptr<T[]> value, const std::size_t valueNumber);

    const std::string_view& variableName() const;
    std::unique_ptr<T[]> value();
    std::size_t valueNumber() const;

private:
    std::string_view _variableName;
    std::unique_ptr<T[]> _value;
    std::size_t _valueNumber;
};

} // namespace cadise

#include "file-io/scene-description/tSdDataUnit.ipp"