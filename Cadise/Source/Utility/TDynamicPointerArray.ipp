#pragma once

#include "Utility/TDynamicPointerArray.h"

#include "Foundation/Assertion.h"

namespace cadise
{

template<typename T>
inline TDynamicPointerArray<T>::TDynamicPointerArray(
    std::unique_ptr<T[]> elements,
    const std::size_t    numElements) :

    _elements(std::move(elements)),
    _numElements(numElements)
{}

template<typename T>
inline T& TDynamicPointerArray<T>::operator [] (const std::size_t index)
{
    CS_ASSERT_LT(index, _numElements);

    return _elements[index];
}

template<typename T>
inline const T& TDynamicPointerArray<T>::operator [] (const std::size_t index) const
{
    CS_ASSERT_LT(index, _numElements);

    return _elements[index];
}

template<typename T>
inline const T* TDynamicPointerArray<T>::rawData() const
{
    return _elements.get();
}

template<typename T>
inline std::size_t TDynamicPointerArray<T>::size() const
{
    return _numElements;
}

} // namespace cadise