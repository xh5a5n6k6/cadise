#pragma once

#include "core/surface/bxdfType.h"

namespace cadise {

class BsdfType {
public:
    BsdfType();
    explicit BsdfType(const BxdfType& type);
    explicit BsdfType(const uint32 type);

    BsdfType operator|(const BsdfType& type) const;

    bool isExactOne(const BxdfType& type) const;
    template<typename... BxdfTypes>
    bool hasAtLeastOne(const BxdfType& type, const BxdfTypes&... types) const;

private:
    uint32 _type;
};

// template header implementation

template<typename... BxdfTypes>
inline bool BsdfType::hasAtLeastOne(const BxdfType& type, const BxdfTypes&... types) const {
    const BxdfType checkTypes[]{type, types...};
    for (auto& checkType : checkTypes) {
        if (_type & static_cast<uint32>(checkType)) {
            return true;
        }
    }

    return false;
}

} // namespace cadise