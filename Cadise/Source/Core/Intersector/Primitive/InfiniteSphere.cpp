#include "core/intersector/primitive/infiniteSphere.h"

#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surface/bsdf/absorberBsdf.h"
#include "core/surfaceDetail.h"
#include "core/texture/mapper/sphericalMapper.h"
#include "fundamental/assertion.h"
#include "math/tAabb3.h"
#include "math/constant.h"

#include <cmath>
#include <limits>

namespace cadise 
{

InfiniteSphere::InfiniteSphere() :
    // HACK
    InfiniteSphere(1000000.0_r) 
{}

InfiniteSphere::InfiniteSphere(const real radius) :
    Primitive(std::make_shared<AbsorberBsdf>()),
    _radius(radius) 
{}

void InfiniteSphere::evaluateBound(AABB3R* const out_bound) const 
{
    CS_ASSERT(out_bound);

    AABB3R bound(Vector3R(-_radius), Vector3R(_radius));
    
    out_bound->set(bound);
}

bool InfiniteSphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const 
{
    if (ray.maxT() < std::numeric_limits<real>::max()) 
    {
        return false;
    }

    ray.setMaxT(_radius);
    primitiveInfo.setPrimitive(this);

    return true;
}

bool InfiniteSphere::isOccluded(const Ray& ray) const
{
    return ray.maxT() >= std::numeric_limits<real>::max();
}

void InfiniteSphere::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceDetail* const out_surface) const
{
    CS_ASSERT(out_surface);

    Vector3R uvw;
    SphericalMapper().mappingToUvw(out_surface->position(), &uvw);

    out_surface->setUvw(uvw);
}

void InfiniteSphere::uvwToPosition(
    const Vector3R& uvw, 
    Vector3R* const out_position) const
{
    CS_ASSERT(out_position);

    const real theta    = (1.0_r - uvw.y()) * constant::pi<real>;
    const real phi      = uvw.x() * constant::two_pi<real>;
    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    const Vector3R direction(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    out_position->set(direction.normalize().mul(_radius));
}

} // namespace cadise