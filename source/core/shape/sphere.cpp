#include "core/shape/sphere.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <limits>

namespace cadise {

Sphere::Sphere(Vector3 center, float radius) : 
    _center(center), _radius(radius) {
    _worldToLocal = Matrix4::translate(-_center.x(), -_center.y(), -_center.z());
}

bool Sphere::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
                _worldToLocal.transformVector(ray.direction()),
                CADISE_RAY_EPSILON, std::numeric_limits<float>::max());

    int isOutside = r.origin().squaredLength() > _radius * _radius;
    float t = r.direction().dot(-r.origin());
    if (isOutside && t < 0.0f) {
        return false;
    }

    float d2 = r.origin().squaredLength() - t * t;
    float s2 = _radius * _radius - d2;
    if (s2 < 0.0f) {
        return false;
    }

    float t0 = t - std::sqrtf(s2);
    float t1 = t + std::sqrtf(s2);
    // Fast comparison without if-else branch
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < r.minT() || t > r.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    /*
        Calculate surface details
    */
    Vector3 point = ray.at(t);
    Vector3 normal = (point - _center).normalize();
    surfaceInfo.setHitPoint(point);
    surfaceInfo.setHitNormal(normal);

    return true;
}

bool Sphere::isOccluded(Ray &ray) {
    Ray r = Ray(_worldToLocal.transformPoint(ray.origin()),
        _worldToLocal.transformVector(ray.direction()),
        CADISE_RAY_EPSILON, std::numeric_limits<float>::max());

    int isOutside = r.origin().squaredLength() > _radius * _radius;
    float t = r.direction().dot(-r.origin());
    if (isOutside && t < 0.0f) {
        return false;
    }

    float d2 = r.origin().squaredLength() - t * t;
    float s2 = _radius * _radius - d2;
    if (s2 < 0.0f) {
        return false;
    }

    float t0 = t - std::sqrtf(s2);
    float t1 = t + std::sqrtf(s2);
    // Fast comparison without if-else branch
    t = isOutside * t0 + (1 - isOutside) * t1;

    if (t < r.minT() || t > r.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    return true;
}

} // namespace cadise