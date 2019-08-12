#include "core/bsdf/bsdfType.h"

namespace cadise {

BsdfType::BsdfType() = default;

BsdfType::BsdfType(const BxdfType& type) :
    _type(static_cast<uint32>(type)) {
}

BsdfType::BsdfType(const uint32 type) {
    _type = type;
}

BsdfType BsdfType::operator|(const BsdfType& type) {
    BsdfType result;
    result._type = _type | type._type;

    return result;
}

} // namespace cadise