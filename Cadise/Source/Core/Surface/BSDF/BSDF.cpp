#include "Core/Surface/BSDF/BSDF.h"

#include <bitset>

namespace cadise
{

BSDF::BSDF() = default;

BSDF::BSDF(const BSDFLobes& lobes) :
    _lobes(lobes)
{
    _components = static_cast<BSDFComponents>(
        std::bitset<32>(lobes.rawLobes()).count());
}

BSDF::BSDF(const BSDFLobes& lobes, const BSDFComponents components) :
    _lobes(lobes),
    _components(components)
{}

BSDF::~BSDF() = default;

const BSDFLobes& BSDF::lobes() const
{
    return _lobes;
}

BSDFComponents BSDF::components() const
{
    return _components;
}

} // namespace cadise