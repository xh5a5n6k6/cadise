#include "core/intersector/primitive/infiniteSphere.h"

#include "core/bsdf/category/absorberBsdf.h"
#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/sphericalMapper.h"
#include "fundamental/assertion.h"
#include "math/aabb.h"
#include "math/constant.h"

#include <cmath>
#include <limits>

namespace cadise {

InfiniteSphere::InfiniteSphere() :
    // HACK
    InfiniteSphere(1000000.0_r) {
}

InfiniteSphere::InfiniteSphere(const real radius) :
    Primitive(std::make_shared<AbsorberBsdf>()),
    _radius(radius) {
}

void InfiniteSphere::evaluateBound(AABB3R* const out_bound) const {
    CADISE_ASSERT(out_bound);

    AABB3R bound(Vector3R(-_radius), Vector3R(_radius));
    
    *out_bound = bound;
}

bool InfiniteSphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    if (ray.maxT() < std::numeric_limits<real>::max()) {
        return false;
    }

    ray.setMaxT(_radius);
    primitiveInfo.setPrimitive(this);

    return true;
}

bool InfiniteSphere::isOccluded(const Ray& ray) const {
    return ray.maxT() >= std::numeric_limits<real>::max();
}

void InfiniteSphere::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceInfo* const   out_surface) const {

    CADISE_ASSERT(out_surface);

    Vector3R uvw;
    SphericalMapper().mappingToUvw(out_surface->point(), &uvw);

    out_surface->setUvw(uvw);
}

void InfiniteSphere::uvwToPosition(
    const Vector3R& uvw, 
    Vector3R* const out_position) const {
    
    CADISE_ASSERT(out_position);

    const real theta    = (1.0_r - uvw.y()) * constant::PI;
    const real phi      = uvw.x() * constant::TWO_PI;
    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    const Vector3R direction(std::cos(phi) * sinTheta,
                             std::sin(phi) * sinTheta,
                             cosTheta);

    *out_position = direction.normalize() * _radius;
}

} // namespace cadise