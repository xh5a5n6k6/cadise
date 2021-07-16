#include "core/surface/bsdf/bsdf.h"

#include <bitset>

namespace cadise {

Bsdf::Bsdf() = default;

Bsdf::Bsdf(const BsdfLobes& lobes) :
    _lobes(lobes) {

    _components = static_cast<BsdfComponents>(
        std::bitset<32>(lobes.rawLobes()).count());
}

Bsdf::Bsdf(const BsdfLobes& lobes, const BsdfComponents components) :
    _lobes(lobes),
    _components(components) {
}

Bsdf::~Bsdf() = default;

const BsdfLobes& Bsdf::lobes() const {
    return _lobes;
}

BsdfComponents Bsdf::components() const {
    return _components;
}

} // namespace cadise