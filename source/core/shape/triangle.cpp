#include "core/shape/triangle.h"

#include "core/ray.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <limits>
#include <random>

namespace cadise {

Triangle::Triangle(Vector3R v1, Vector3R v2, Vector3R v3) :
    _v1(v1), _v2(v2), _v3(v3) {
    _e1 = _v2 - _v1;
    _e2 = _v3 - _v1;
}

AABB3R Triangle::bound() const {
    return AABB3R(_v1).unionWith(_v2).unionWith(_v3);
}

bool Triangle::isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) const {
    Vector3R D = ray.direction();
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    if (D.dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
    }
    Vector3R T = ray.origin() - _v1;
    Vector3R Q = T.cross(e1);
    Vector3R P = D.cross(e2);

    real denominator = P.dot(e1);
    if (denominator - 0.0_r < std::numeric_limits<real>::epsilon()) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(e2) * invDenominator;
    real u = P.dot(T) * invDenominator;
    real v = Q.dot(D) * invDenominator;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    // Calculate surface details
    Vector3R point = ray.at(t);
    Vector3R normal = e1.cross(e2).normalize();
    surfaceInfo.setPoint(point);
    surfaceInfo.setNormal(normal);

    return true;
}

bool Triangle::isOccluded(Ray &ray) const {
    Vector3R D = ray.direction();
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    if (D.dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
    }
    Vector3R T = ray.origin() - _v1;
    Vector3R Q = T.cross(e1);
    Vector3R P = D.cross(e2);

    real denominator = P.dot(e1);
    if (denominator - 0.0_r < std::numeric_limits<real>::epsilon()) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(e2) * invDenominator;
    real u = P.dot(T) * invDenominator;
    real v = Q.dot(D) * invDenominator;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    ray.setMaxT(t);

    return true;
}

void Triangle::sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface) const {
    // TODO
    // improve sample point on triangle
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<real> disS(0.0_r, 1.0_r);
    std::uniform_real_distribution<real> disT(0.0_r, 1.0_r);
    real s;
    real t;

    // Use rejection method
    do {
        s = disS(gen);
        t = disT(gen);
    } while (s + t >= 1.0_r);

    Vector3R point = _v1 + s * _e1 + t * _e2;
    Vector3R direction = point - inSurface.point();
    Vector3R normal;
    if (direction.dot(_e1.cross(_e2)) > 0.0_r) {
        normal = _e2.cross(_e1).normalize();
    }
    else {
        normal = _e1.cross(_e2).normalize();
    }

    outSurface.setPoint(point);
    outSurface.setNormal(normal);
}

} // namespace cadise