#include "core/shape/rectangle.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include <limits>
#include <random>

namespace cadise {

Rectangle::Rectangle(Vector3F v1, Vector3F v2, Vector3F v3) :
    _v1(v1), _v2(v2), _v3(v3) {
    _e1 = _v1 - _v2;
    _e2 = _v3 - _v2;
}

AABB3F Rectangle::bound() {
    return AABB3F(_v1).unionWith(_v2).unionWith(_v3).unionWith(_v2 + _e1 + _e2);
}

bool Rectangle::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) {
    if (ray.direction().dot(_e1.cross(_e2)) > 0.0f) {
        _e1.swap(_e2);
    }
    Vector3F normal = _e1.cross(_e2).normalize();
    float t = (normal.dot(_v2) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0f || t > ray.maxT()) {
        return false;
    }

    Vector3F vectorOnPlane = ray.at(t) - _v2;
    float projection1 = vectorOnPlane.dot(_e1.normalize());
    float projection2 = vectorOnPlane.dot(_e2.normalize());
    if (projection1 < 0.0f || projection1 > _e1.length() ||
        projection2 < 0.0f || projection2 > _e2.length()) {
        return false;
    }

    ray.setMaxT(t);

    //  Calculate surface details
    surfaceInfo.setPoint(ray.at(t));
    surfaceInfo.setNormal(normal);

    return true;
}

bool Rectangle::isOccluded(Ray &ray) {
    if (ray.direction().dot(_e1.cross(_e2)) > 0.0f) {
        _e1.swap(_e2);
    }
    Vector3F normal = _e1.cross(_e2).normalize();
    float t = (normal.dot(_v2) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0f || t > ray.maxT()) {
        return false;
    }

    Vector3F vectorOnPlane = ray.at(t) - _v2;
    float projection1 = vectorOnPlane.dot(_e1.normalize());
    float projection2 = vectorOnPlane.dot(_e2.normalize());
    if (projection1 < 0.0f || projection1 > _e1.length() ||
        projection2 < 0.0f || projection2 > _e2.length()) {
        return false;
    }

    ray.setMaxT(t);

    return true;
}

void Rectangle::sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) {
    if (_e1.length() < _e2.length()) {
        _e1.swap(_e2);
    }
    float longWidth = _e1.length();
    float shortWidth = _e2.length();

    // TODO
    // improve sample point on rectangle
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<float> disS(0.0f, 1.0f);
    std::uniform_real_distribution<float> disT(0.0f, 1.0f);
    float s;
    float t;

    // Use rejection method
    do {
        s = disS(gen);
        t = disT(gen);
    } while (t > shortWidth / longWidth);

    Vector3F point = _v2 + s * _e1 + t * _e1.length() * _e2.normalize();
    Vector3F direction = point - inSurface.point();
    if (direction.dot(_e1.cross(_e2)) > 0.0f) {
        _e1.swap(_e2);
    }
    Vector3F normal = _e1.cross(_e2).normalize();

    outSurface.setPoint(point);
    outSurface.setNormal(normal);
}

float Rectangle::samplePdf() {
    assert(area() > 0.0f);

    return 1.0f / area();
}

float Rectangle::area() {
    return _e1.length() * _e2.length();
}

} // namespace cadise