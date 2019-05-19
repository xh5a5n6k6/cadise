#include "core/shape/rectangle.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include <limits>
#include <random>

namespace cadise {

Rectangle::Rectangle(Vector3 v1, Vector3 v2, Vector3 v3) {
    _vertex[0] = v1;
    _vertex[1] = v2;
    _vertex[2] = v3;
}

bool Rectangle::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) {
    Vector3 E1 = _vertex[0] - _vertex[1];
    Vector3 E2 = _vertex[2] - _vertex[1];
    if (Dot(ray.direction(), Cross(E1, E2)) > 0.0f) {
        E1.swap(E2);
    }
    Vector3 normal = Cross(E1, E2).normalize();
    float t = (Dot(normal, _vertex[1]) - Dot(normal, ray.origin())) / Dot(normal, ray.direction());
    if (t < 0.0f || t > ray.maxT()) {
        return false;
    }

    Vector3 vectorOnPlane = ray.at(t) - _vertex[1];
    float projection1 = Dot(vectorOnPlane, E1.normalize());
    float projection2 = Dot(vectorOnPlane, E2.normalize());
    if (projection1 < 0.0f || projection1 > E1.length() ||
        projection2 < 0.0f || projection2 > E2.length()) {
        return false;
    }

    ray.setMaxT(t);

    /*
        Calculate surface details
    */
    surfaceInfo.setHitPoint(ray.at(t));
    surfaceInfo.setHitNormal(normal);

    return true;
}

bool Rectangle::isOccluded(Ray &ray) {
    Vector3 E1 = _vertex[0] - _vertex[1];
    Vector3 E2 = _vertex[2] - _vertex[1];
    if (Dot(ray.direction(), Cross(E1, E2)) > 0.0f) {
        E1.swap(E2);
    }
    Vector3 normal = Cross(E1, E2).normalize();
    float t = (Dot(normal, _vertex[1]) - Dot(normal, ray.origin())) / Dot(normal, ray.direction());
    if (t < 0.0f || t > ray.maxT()) {
        return false;
    }

    Vector3 vectorOnPlane = ray.at(t) - _vertex[1];
    float projection1 = Dot(vectorOnPlane, E1.normalize());
    float projection2 = Dot(vectorOnPlane, E2.normalize());
    if (projection1 < 0.0f || projection1 > E1.length() ||
        projection2 < 0.0f || projection2 > E2.length()) {
        return false;
    }

    ray.setMaxT(t);

    return true;
}

Vector3 Rectangle::sampleSurfacePoint() {
    Vector3 E1 = _vertex[0] - _vertex[1];
    Vector3 E2 = _vertex[2] - _vertex[1];
    if (E1.length() < E2.length()) {
        E1.swap(E2);
    }
    float longWidth = E1.length();
    float shortWidth = E2.length();

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

    return _vertex[1] + s * E1 + t * E1.length() * E2.normalize();
}

} // namespace cadise