#include "core/intersector/primitive/category/triangle.h"

#include "core/intersector/primitive/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/textureMapper.h"

#include "fundamental/assertion.h"

#include "math/constant.h"
#include "math/random.h"

#include <cmath>
#include <limits>

namespace cadise {

Triangle::Triangle(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& vA, const Vector3R& vB, const Vector3R& vC) :
    Primitive(bsdf), 
    _vA(vA),
    _vB(vB), 
    _vC(vC) {
    
    _eA = _vB - _vA;
    _eB = _vC - _vA;

    _uvwA = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(1.0_r, 1.0_r, 0.0_r);
}

AABB3R Triangle::bound() const {
    return AABB3R(_vA).unionWith(_vB).unionWith(_vC).expand(0.0001_r);
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R D = ray.direction();
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    bool isBackSide = false;
    if (D.dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
        isBackSide = true;
    }
    Vector3R T = ray.origin() - _vA;
    Vector3R Q = T.cross(eA);
    Vector3R P = D.cross(eB);

    real denominator = P.dot(eA);
    if (denominator - 0.0_r < std::numeric_limits<real>::epsilon()) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(eB) * invDenominator;
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

bool Triangle::isOccluded(Ray& ray) const {
    Vector3R D = ray.direction();
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    if (D.dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
    }
    Vector3R T = ray.origin() - _vA;
    Vector3R Q = T.cross(eA);
    Vector3R P = D.cross(eB);

    real denominator = P.dot(eA);
    if (denominator - 0.0_r < std::numeric_limits<real>::epsilon()) {
        return false;
    }

    real invDenominator = 1.0_r / denominator;
    real t = Q.dot(eB) * invDenominator;
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
    Vector3R normal = _eA.cross(_eB).normalize();
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
        const Vector3R P = surfaceInfo.point();
        const Vector3R AP = P - _vA;
        const Vector3R AB = _eA;
        const Vector3R AC = _eB;
        
        real invDenominator = 1.0_r / AB.cross(AC).length();

        CADISE_ASSERT(std::isfinite(invDenominator));

        real u = AP.cross(AC).length() * invDenominator;
        real v = AP.cross(AB).length() * invDenominator;
        uvw = (1.0_r - u - v) * _uvwA +
              u * _uvwB +
              v * _uvwC;
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
        s = random::get1D();
        t = random::get1D();
    } while (s + t >= 1.0_r);

    Vector3R eA = _eA;
    Vector3R eB = _eB;
    Vector3R point = _vA + s * eA + t * eB;
    Vector3R direction = point - inSurface.point();
    if (direction.dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
    }
    Vector3R normal = eA.cross(eB).normalize();

    outSurface.setPoint(point);
    outSurface.setGeometryNormal(normal);
}

real Triangle::samplePdfA(const Vector3R& position) const {
    CADISE_ASSERT(area() > 0.0_r);

    return 1.0_r / area();
}

real Triangle::area() const {
    return 0.5_r * _eA.cross(_eB).length();
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