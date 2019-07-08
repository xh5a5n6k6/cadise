#include "core/intersector/primitive/sphere.h"

#include "core/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceGeometryInfo.h"

#include "math/constant.h"

#include <limits>

namespace cadise {

Sphere::Sphere(const std::shared_ptr<BSDF>& bsdf, const Vector3R& center, const real radius) :
    Primitive(bsdf), 
    _center(center),
    _radius(radius) {

    _worldToLocal = Matrix4::translate(-_center.x(), -_center.y(), -_center.z());
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

    return true;
}

bool Sphere::isOccluded(Ray& ray) const {
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

    return true;
}

void Sphere::evaluateGeometryDetail(const PrimitiveInfo& primitiveInfo, SurfaceGeometryInfo& geometryInfo) const {
    Vector3R normal = (geometryInfo.point() - _center).normalize();
    geometryInfo.setNormal(normal);
}

void Sphere::evaluteShadingDetail(SurfaceShadingInfo& shadingInfo) const {

}

void Sphere::sampleSurface(const SurfaceGeometryInfo& inSurface, SurfaceGeometryInfo& outSurface) const {

}

real Sphere::samplePdfA(const Vector3R& position) const {
    assert(area() > 0.0_r);

    return 1.0_r / area();
}

real Sphere::area() const {
    return constant::FOUR_PI * _radius * _radius;
}

} // namespace cadise