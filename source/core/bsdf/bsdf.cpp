#include "core/bsdf/bsdf.h"

namespace cadise {

Bsdf::Bsdf() = default;

Bsdf::Bsdf(const BsdfType& type) :
    _type(type) {
}

Bsdf::~Bsdf() = default;

const BsdfType& Bsdf::type() const {
    return _type;
}

} // namespace cadise