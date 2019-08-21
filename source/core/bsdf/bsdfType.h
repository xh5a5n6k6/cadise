#pragma once

#include "core/bsdf/bxdfType.h"

namespace cadise {

class BsdfType {
public:
    BsdfType();
    BsdfType(const BxdfType& type);
    BsdfType(const uint32 type);

    BsdfType operator|(const BsdfType& type);

    template<typename... BsdfTypes>
    bool isAtLeastOne(const BsdfType& type, const BsdfTypes&... types) const;

private:
    uint32 _type;
};

// template header implementation

template<typename... BsdfTypes>
inline bool BsdfType::isAtLeastOne(const BsdfType& type, const BsdfTypes&... types) const {
    BsdfType checkTypes[]{ type, types... };
    for (auto& checkType : checkTypes) {
        if (_type & checkType._type) {
            return true;
        }
    }

    return false;
}

} // namespace cadise