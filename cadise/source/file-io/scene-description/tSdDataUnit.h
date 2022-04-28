#pragma once

#include "math/type/fundamentalType.h"
#include "utility/tDynamicPointerArray.h"

#include <string>

namespace cadise 
{

template<typename T>
class TSdDataUnit 
{
public:
    TSdDataUnit(
        const std::string&   name, 
        std::unique_ptr<T[]> value, 
        const std::size_t    numValues);

    TSdDataUnit(
        const std::string&        name,
        TDynamicPointerArray<T>&& value);

    const std::string& name() const;
    TDynamicPointerArray<T> value();
    std::size_t numValues() const;

private:
    std::string             _name;
    TDynamicPointerArray<T> _value;

    bool _isLookedUp;
};

} // namespace cadise

#include "file-io/scene-description/tSdDataUnit.ipp"