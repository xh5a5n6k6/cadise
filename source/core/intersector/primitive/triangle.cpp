#include "core/intersector/primitive/triangle.h"

#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/textureMapper.h"

#include "fundamental/assertion.h"

#include "math/aabb.h"
#include "math/constant.h"
#include "math/random.h"

#include <limits>

namespace cadise {

Triangle::Triangle(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& vA, const Vector3R& vB, const Vector3R& vC) :
    Primitive(bsdf), 
    _vA(vA),
    _vB(vB), 
    _vC(vC) {
    
    _eAB = _vB - _vA;
    _eAC = _vC - _vA;

    _uvwA = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
}

AABB3R Triangle::bound() const {
    return AABB3R(_vA).unionWith(_vB).unionWith(_vC).expand(0.0001_r);
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R D = ray.direction();
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;
    bool isBackSide = false;
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
        isBackSide = true;
    }
    Vector3R T = ray.origin() - _vA;
    Vector3R Q = T.cross(eAB);
    Vector3R P = D.cross(eAC);

    real denominator = P.dot(eAB);
    if (denominator == 0.0_r) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(eAC) * invDenominator;
    real u = P.dot(T) * invDenominator;
    real v = Q.dot(D) * invDenominator;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isBackSide);

    return true;
}

bool Triangle::isOccluded(const Ray& ray) const {
    Vector3R D = ray.direction();
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
    }
    Vector3R T = ray.origin() - _vA;
    Vector3R Q = T.cross(eAB);
    Vector3R P = D.cross(eAC);

    real denominator = P.dot(eAB);
    if (denominator == 0.0_r) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(eAC) * invDenominator;
    real u = P.dot(T) * invDenominator;
    real v = Q.dot(D) * invDenominator;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    return true;
}

void Triangle::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
    Vector3R normal = _eAB.cross(_eAC).normalize();
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
        // TODO : integrate with intersecting/occluded 
        //        barycentric coordinate calculation
        Vector3R v0 = _vB - _vA;
        Vector3R v1 = _vC - _vA;
        Vector3R v2 = surfaceInfo.point() - _vA;
        real d00 = v0.dot(v0);
        real d01 = v0.dot(v1);
        real d11 = v1.dot(v1);
        real d20 = v2.dot(v0);
        real d21 = v2.dot(v1);
        real denominator = d00 * d11 - d01 * d01;
        if (denominator == 0.0_r) {
            uvw = Vector3R(0.0_r, 0.0_r, 0.0_r);
        }
        else {
            real invDenominator = 1.0_r / denominator;
            real u = (d11 * d20 - d01 * d21) * invDenominator;
            real v = (d00 * d21 - d01 * d20) * invDenominator;

            uvw = (1.0_r - u - v) * _uvwA +
                  u * _uvwB +
                  v * _uvwC;
        }
        surfaceInfo.setUvw(uvw);
    }
}

void Triangle::sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const {
    // TODO
    // improve sample point on triangle
    real s;
    real t;

    // Use rejection method
    do {
        s = random::nextReal();
        t = random::nextReal();
    } while (s + t >= 1.0_r);

    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;
    Vector3R point = _vA + s * eAB + t * eAC;
    Vector3R direction = point - inSurface.point();
    if (direction.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
    }
    Vector3R normal = eAB.cross(eAC).normalize();

    outSurface.setPoint(point);
    outSurface.setGeometryNormal(normal);
}

real Triangle::samplePdfA(const Vector3R& position) const {
    CADISE_ASSERT(area() > 0.0_r);

    return 1.0_r / area();
}

real Triangle::area() const {
    return 0.5_r * _eAB.cross(_eAC).length();
}

void Triangle::setUvwA(const Vector3R& uvwA) {
    _uvwA = uvwA;
}

void Triangle::setUvwB(const Vector3R& uvwB) {
    _uvwB = uvwB;
}

void Triangle::setUvwC(const Vector3R& uvwC) {
    _uvwC = uvwC;
}

} // namespace cadise