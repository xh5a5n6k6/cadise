#include "core/bsdf/bsdfType.h"

namespace cadise {

BSDFType::BSDFType() = default;

BSDFType::BSDFType(const BxDF_Type& type) :
    _type(static_cast<uint32>(type)) {
}

BSDFType::BSDFType(const uint32 type) {
    _type = type;
}

BSDFType BSDFType::operator|(const BSDFType& type) {
    BSDFType result;
    result._type = _type | type._type;

    return result;
}

} // namespace cadise