#include "file-io/scene-description/tSdDataUnit.h"

namespace cadise
{

template<typename T>
inline TSdDataUnit<T>::TSdDataUnit() :
    _variableName(""),
    _value(nullptr),
    _valueNumber(0) 
{}

template<typename T>
inline TSdDataUnit<T>::TSdDataUnit(const std::string_view& name, std::unique_ptr<T[]> value, const std::size_t valueNumber) :
    _variableName(name),
    _value(std::move(value)),
    _valueNumber(valueNumber)
{}

template<typename T>
inline const std::string_view& TSdDataUnit<T>::variableName() const 
{
    return _variableName;
}

template<typename T>
inline std::unique_ptr<T[]> TSdDataUnit<T>::value()
{
    return std::move(_value);
}

template<typename T>
inline std::size_t TSdDataUnit<T>::valueNumber() const 
{
    return _valueNumber;
}

} // namespace cadise