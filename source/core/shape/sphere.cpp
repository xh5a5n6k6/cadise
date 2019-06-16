#include "core/shape/sphere.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <limits>

namespace cadise {

Sphere::Sphere(const Vector3R center, const real radius) : 
    _center(center), _radius(radius) {
    _worldToLocal = Matrix4::translate(-_center.x(), -_center.y(), -_center.z());
}

AABB3R Sphere::bound() const {
    return AABB3R(_center - _radius - 0.0001_r, _center + _radius + 0.0001_r);
}

bool Sphere::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) const {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
                _worldToLocal.transformVector(ray.direction()),
                constant::RAY_EPSILON, std::numeric_limits<real>::max());

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

    // Calculate surface details
    Vector3R point = ray.at(t);
    Vector3R normal = (point - _center).normalize();
    surfaceInfo.setPoint(point);
    surfaceInfo.setNormal(normal);

    return true;
}

bool Sphere::isOccluded(Ray &ray) const {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
                _worldToLocal.transformVector(ray.direction()),
                constant::RAY_EPSILON, std::numeric_limits<real>::max());

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

} // namespace cadise