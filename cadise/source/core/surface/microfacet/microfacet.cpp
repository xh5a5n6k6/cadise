#include "core/surface/microfacet/microfacet.h"

#include "core/surface/microfacet/tRoughnessMapper.h"
#include "math/tVector3.h"

namespace cadise 
{

Microfacet::~Microfacet() = default;

bool Microfacet::_isShadowingMaskingValid(
    const Vector3R& V,
    const Vector3R& L,
    const Vector3R& N,
    const Vector3R& H) const 
{
    const real VdotH = V.dot(H);
    const real VdotN = V.dot(N);
    const real LdotH = L.dot(H);
    const real LdotN = L.dot(N);

    return (VdotH * VdotN > 0.0_r) && (LdotH * LdotN > 0.0_r);
}

real Microfacet::_roughnessToAlpha(const real roughness) const 
{
    // TODO: make mapping mode an user-control variable
    return TRoughnessMapper<ERoughnessMapMode::Square>().map(roughness);
}

} // namespace cadise