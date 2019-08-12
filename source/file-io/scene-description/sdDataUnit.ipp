#include "file-io/scene-description/sdDataUnit.h"

namespace cadise {

template<typename T>
inline SdDataUnit<T>::SdDataUnit() :
    _variableName(""),
    _value(nullptr),
    _valueNumber(0) {
}

template<typename T>
inline SdDataUnit<T>::SdDataUnit(const std::string_view& name, std::unique_ptr<T[]> value, const int32 valueNumber) :
    _variableName(name),
    _value(std::move(value)),
    _valueNumber(valueNumber) {
}

template<typename T>
inline std::string_view SdDataUnit<T>::variableName() const {
    return _variableName;
}

template<typename T>
inline std::unique_ptr<T[]> SdDataUnit<T>::value() {
    return std::move(_value);
}

template<typename T>
inline int32 SdDataUnit<T>::valueNumber() const {
    return _valueNumber;
}

} // namespace cadise