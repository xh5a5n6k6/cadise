#include "core/intersector/primitive/sphere.h"

#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/sphericalMapper.h"

#include "fundamental/assertion.h"

#include "math/aabb.h"
#include "math/constant.h"
#include "math/random.h"

#include <limits>

namespace cadise {

Sphere::Sphere(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& center, const real radius) :
    Primitive(bsdf), 
    _center(center),
    _radius(radius) {

    _worldToLocal = Matrix4::translate(-_center.x(), -_center.y(), -_center.z());
    _tmptextureMapper = std::make_shared<SphericalMapper>();
}

AABB3R Sphere::bound() const {
    return AABB3R(_center - _radius, _center + _radius).expand(0.0001_r);
}

bool Sphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
                _worldToLocal.transformVector(ray.direction()),
                constant::RAY_EPSILON, 
                std::numeric_limits<real>::max());

    int32 isOutside = r.origin().lengthSquared() > _radius * _radius;
    real t = r.direction().dot(-r.origin());
    if (isOutside && t < 0.0_r) {
        return false;
    }

    real d2 = r.origin().lengthSquared() - t * t;
    real s2 = _radius * _radius - d2;
    if (s2 < 0.0_r) {
        return false;
    }

    real t0 = t - std::sqrt(s2);
    real t1 = t + std::sqrt(s2);
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < r.minT() || t > r.maxT()) {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isOutside == 0);

    return true;
}

bool Sphere::isOccluded(const Ray& ray) const {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
                _worldToLocal.transformVector(ray.direction()),
                constant::RAY_EPSILON, 
                std::numeric_limits<real>::max());

    int32 isOutside = r.origin().lengthSquared() > _radius * _radius;
    real t = r.direction().dot(-r.origin());
    if (isOutside && t < 0.0_r) {
        return false;
    }

    real d2 = r.origin().lengthSquared() - t * t;
    real s2 = _radius * _radius - d2;
    if (s2 < 0.0_r) {
        return false;
    }

    real t0 = t - std::sqrt(s2);
    real t1 = t + std::sqrt(s2);
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < r.minT() || t > r.maxT()) {
        return false;
    }

    return true;
}

void Sphere::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
    Vector3R normal = (surfaceInfo.point() - _center).normalize();
    surfaceInfo.setFrontNormal(normal);

    normal = (primitiveInfo.isBackSide()) ? normal.composite() : normal;
    surfaceInfo.setGeometryNormal(normal);
    surfaceInfo.setShadingNormal(normal);

    Vector3R uvw;
    if (_textureMapper) {
        uvw = _textureMapper->mappingToUvw(surfaceInfo);
        surfaceInfo.setUvw(uvw);
    }
    else {
        uvw = _tmptextureMapper->mappingToUvw(surfaceInfo);
        surfaceInfo.setUvw(uvw);
    }
}

void Sphere::sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const {

}

real Sphere::samplePdfA(const Vector3R& position) const {
    CADISE_ASSERT(area() > 0.0_r);

    return 1.0_r / area();
}

real Sphere::area() const {
    return constant::FOUR_PI * _radius * _radius;
}

} // namespace cadise