#include "core/bsdf/bsdf.h"

namespace cadise {

Bsdf::Bsdf(const BsdfType& type) :
    _type(type) {
}

BsdfType Bsdf::type() const {
    return _type;
}

} // namespace cadise