#include "core/shape/triangle.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <limits>

namespace cadise {

Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
    _vertex[0] = v1;
    _vertex[1] = v2;
    _vertex[2] = v3;
}

bool Triangle::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) {
    Vector3 D = ray.direction();
    Vector3 E1 = _vertex[1] - _vertex[0];
    Vector3 E2 = _vertex[2] - _vertex[0];
    if (Dot(D, Cross(E1, E2)) > 0.0f)
        E1.swap(E2);
    Vector3 T = ray.origin() - _vertex[0];
    Vector3 Q = Cross(T, E1);
    Vector3 P = Cross(D, E2);

    float denominator = Dot(P, E1);
    if (denominator - 0.0f < std::numeric_limits<float>::epsilon())
        return false;

    float invDenominator = 1.0f / denominator;
    float t = Dot(Q, E2) * invDenominator;
    float u = Dot(P, T) * invDenominator;
    float v = Dot(Q, D) * invDenominator;

    if (u < 0.0f || v < 0.0f || u + v > 1.0f)
        return false;

    if (t < ray.minT() || t > ray.maxT())
        return false;

    ray.setMaxT(t);

    /*
        Calculate surface details
    */
    Vector3 point = ray.at(ray.maxT());
    Vector3 normal = Cross(E1, E2).normalize();
    surfaceInfo.setHitPoint(point);
    surfaceInfo.setHitNormal(normal);

    return true;
}

bool Triangle::isOccluded(Ray &ray) {
    Vector3 D = ray.direction();
    Vector3 E1 = _vertex[1] - _vertex[0];
    Vector3 E2 = _vertex[2] - _vertex[0];
    if (Dot(D, Cross(E1, E2)) > 0.0f)
        E1.swap(E2);
    Vector3 T = ray.origin() - _vertex[0];
    Vector3 Q = Cross(T, E1);
    Vector3 P = Cross(D, E2);

    float denominator = Dot(P, E1);
    if (denominator - 0.0f < std::numeric_limits<float>::epsilon())
        return false;

    float invDenominator = 1.0f / denominator;
    float t = Dot(Q, E2) * invDenominator;
    float u = Dot(P, T) * invDenominator;
    float v = Dot(Q, D) * invDenominator;

    if (u < 0.0f || v < 0.0f || u + v > 1.0f)
        return false;

    if (t < ray.minT() || t > ray.maxT())
        return false;

    ray.setMaxT(t);

    return true;
}

} // namespace cadise