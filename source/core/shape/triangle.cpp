#include "core/shape/triangle.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <limits>
#include <random>

namespace cadise {

Triangle::Triangle(Vector3F v1, Vector3F v2, Vector3F v3) {
    _vertex[0] = v1;
    _vertex[1] = v2;
    _vertex[2] = v3;
}

bool Triangle::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) {
    Vector3F D = ray.direction();
    Vector3F E1 = _vertex[1] - _vertex[0];
    Vector3F E2 = _vertex[2] - _vertex[0];
    if (D.dot(E1.cross(E2)) > 0.0f) {
        E1.swap(E2);
    }
    Vector3F T = ray.origin() - _vertex[0];
    Vector3F Q = T.cross(E1);
    Vector3F P = D.cross(E2);

    float denominator = P.dot(E1);
    if (denominator - 0.0f < std::numeric_limits<float>::epsilon()) {
        return false;
    }

    float invDenominator = 1.0f / denominator;
    float t = Q.dot(E2) * invDenominator;
    float u = P.dot(T) * invDenominator;
    float v = Q.dot(D) * invDenominator;

    if (u < 0.0f || v < 0.0f || u + v > 1.0f) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    /*
        Calculate surface details
    */
    Vector3F point = ray.at(t);
    Vector3F normal = E1.cross(E2).normalize();
    surfaceInfo.setPoint(point);
    surfaceInfo.setNormal(normal);

    return true;
}

bool Triangle::isOccluded(Ray &ray) {
    Vector3F D = ray.direction();
    Vector3F E1 = _vertex[1] - _vertex[0];
    Vector3F E2 = _vertex[2] - _vertex[0];
    if (D.dot(E1.cross(E2)) > 0.0f) {
        E1.swap(E2);
    }
    Vector3F T = ray.origin() - _vertex[0];
    Vector3F Q = T.cross(E1);
    Vector3F P = D.cross(E2);

    float denominator = P.dot(E1);
    if (denominator - 0.0f < std::numeric_limits<float>::epsilon()) {
        return false;
    }

    float invDenominator = 1.0f / denominator;
    float t = Q.dot(E2) * invDenominator;
    float u = P.dot(T) * invDenominator;
    float v = Q.dot(D) * invDenominator;

    if (u < 0.0f || v < 0.0f || u + v > 1.0f) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    return true;
}

void Triangle::sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) {
    // TODO
    // improve sample point on triangle
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
    } while (s + t >= 1.0f);

    Vector3F E1 = _vertex[1] - _vertex[0];
    Vector3F E2 = _vertex[2] - _vertex[0];

    Vector3F point = _vertex[0] + s * E1 + t * E2;
    Vector3F direction = point - inSurface.point();
    if (direction.dot(E1.cross(E2)) > 0.0f) {
        E1.swap(E2);
    }
    Vector3F normal = E1.cross(E2).normalize();

    outSurface.setPoint(point);
    outSurface.setNormal(normal);
}

} // namespace cadise