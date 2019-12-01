#pragma once

#include "core/bsdf/bxdfType.h"

namespace cadise {

class BsdfType {
public:
    BsdfType();
    BsdfType(const BxdfType& type);
    BsdfType(const uint32 type);

    BsdfType operator|(const BsdfType& type) const;

    bool isExactOne(const BxdfType& type) const;
    template<typename... BxdfTypes>
    bool isAtLeastOne(const BxdfType& type, const BxdfTypes&... types) const;

private:
    uint32 _type;
};

// template header implementation

template<typename... BxdfTypes>
inline bool BsdfType::isAtLeastOne(const BxdfType& type, const BxdfTypes&... types) const {
    const BxdfType checkTypes[]{type, types...};
    for (auto& checkType : checkTypes) {
        if (_type & static_cast<uint32>(checkType)) {
            return true;
        }
    }

    return false;
}

} // namespace cadise