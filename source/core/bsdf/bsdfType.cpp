#include "core/bsdf/bsdfType.h"

namespace cadise {

BsdfType::BsdfType() :
    BsdfType(BxdfType::ABSORB) {
}

BsdfType::BsdfType(const BxdfType& type) :
    _type(static_cast<uint32>(type)) {
}

BsdfType::BsdfType(const uint32 type) {
    _type = type;
}

BsdfType BsdfType::operator|(const BsdfType& type) const {
    BsdfType result;
    result._type = _type | type._type;

    return result;
}

bool BsdfType::isExactOne(const BxdfType& type)const {
    return _type & static_cast<uint32>(type);
}

} // namespace cadise