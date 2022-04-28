#pragma once

#include "file-io/scene-description/tSdDataUnit.h"

#include "fundamental/assertion.h"

namespace cadise
{

template<typename T>
inline TSdDataUnit<T>::TSdDataUnit(
    const std::string&   name,
    std::unique_ptr<T[]> value,
    const std::size_t    numValues) :

    TSdDataUnit(name, TDynamicPointerArray<T>(std::move(value), numValues))
{}

template<typename T>
inline TSdDataUnit<T>::TSdDataUnit(
    const std::string&        name,
    TDynamicPointerArray<T>&& value) :

    _name(name),
    _value(std::move(value)),
    _isLookedUp(false)
{}

template<typename T>
inline const std::string& TSdDataUnit<T>::name() const 
{
    return _name;
}

template<typename T>
inline TDynamicPointerArray<T> TSdDataUnit<T>::value()
{
    CS_ASSERT(!_isLookedUp);

    _isLookedUp = true;

    return std::move(_value);
}

template<typename T>
inline std::size_t TSdDataUnit<T>::numValues() const 
{
    CS_ASSERT(!_isLookedUp);

    return _value.size();
}

} // namespace cadise