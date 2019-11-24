#include "core/intersector/primitive/sphere.h"

#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/sphericalMapper.h"

#include "fundamental/assertion.h"

#include "math/aabb.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/transform.h"

#include <limits>

namespace cadise {

Sphere::Sphere(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& center, const real radius) :
    Primitive(bsdf), 
    _center(center),
    _radius(radius) {

    _worldToLocal = std::make_shared<Transform>(Matrix4::translate(center.reverse()));
    _tmptextureMapper = std::make_shared<SphericalMapper>();
}

AABB3R Sphere::bound() const {
    return AABB3R(_center - _radius, _center + _radius).expand(0.0001_r);
}

bool Sphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Ray localRay(_worldToLocal->transformPoint(ray.origin()),
                 _worldToLocal->transformVector(ray.direction()),
                 ray.minT(), 
                 ray.maxT());

    const int32 isOutside = localRay.origin().lengthSquared() > _radius * _radius;
    real t = localRay.direction().dot(localRay.origin().reverse());
    if (isOutside && t < 0.0_r) {
        return false;
    }

    const real d2 = localRay.origin().lengthSquared() - t * t;
    const real s2 = _radius * _radius - d2;
    if (s2 < 0.0_r) {
        return false;
    }

    const real t0 = t - std::sqrt(s2);
    const real t1 = t + std::sqrt(s2);
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < localRay.minT() || t > localRay.maxT()) {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isOutside == 0);

    return true;
}

bool Sphere::isOccluded(const Ray& ray) const {
    Ray localRay(_worldToLocal->transformPoint(ray.origin()),
                 _worldToLocal->transformVector(ray.direction()),
                 ray.minT(),
                 ray.maxT());

    const int32 isOutside = localRay.origin().lengthSquared() > _radius * _radius;
    real t = localRay.direction().dot(localRay.origin().reverse());
    if (isOutside && t < 0.0_r) {
        return false;
    }

    const real d2 = localRay.origin().lengthSquared() - t * t;
    const real s2 = _radius * _radius - d2;
    if (s2 < 0.0_r) {
        return false;
    }

    const real t0 = t - std::sqrt(s2);
    const real t1 = t + std::sqrt(s2);
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < localRay.minT() || t > localRay.maxT()) {
        return false;
    }

    return true;
}

void Sphere::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
    Vector3R normal = (surfaceInfo.point() - _center).normalize();
    surfaceInfo.setFrontNormal(normal);

    normal = (primitiveInfo.isBackSide()) ? normal.reverse() : normal;
    surfaceInfo.setGeometryNormal(normal);
    surfaceInfo.setShadingNormal(normal);

    Vector3R uvw;
    if (_textureMapper) {
        _textureMapper->mappingToUvw(surfaceInfo.frontNormal(), &uvw);
        surfaceInfo.setUvw(uvw);
    }
    else {
        _tmptextureMapper->mappingToUvw(surfaceInfo.frontNormal(), &uvw);
        surfaceInfo.setUvw(uvw);
    }
}

void Sphere::sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const {
    // TODO: implement here
}

real Sphere::samplePdfA(const Vector3R& position) const {
    return 1.0_r / area();
}

real Sphere::area() const {
    return constant::FOUR_PI * _radius * _radius;
}

} // namespace cadise