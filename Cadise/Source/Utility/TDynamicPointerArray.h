#pragma once

#include "utility/iMovableOnly.h"

#include <memory>

namespace cadise
{

template<typename T>
class TDynamicPointerArray : private IMovableOnly
{
public:
    TDynamicPointerArray(
        std::unique_ptr<T[]> elements,
        const std::size_t    numElements);

    T& operator [] (const std::size_t index);
    const T& operator [] (const std::size_t index) const;

    const T* rawData() const;
    std::size_t size() const;

private:
    std::unique_ptr<T[]> _elements;
    std::size_t          _numElements;
};

} // namespace cadise

#include "utility/tDynamicPointerArray.ipp"