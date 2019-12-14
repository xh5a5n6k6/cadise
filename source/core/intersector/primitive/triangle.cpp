#include "core/intersector/primitive/triangle.h"

#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/textureMapper.h"
#include "fundamental/assertion.h"
#include "math/aabb.h"
#include "math/constant.h"
#include "math/random.h"

namespace cadise {

Triangle::Triangle(const std::shared_ptr<Bsdf>& bsdf, 
                   const Vector3R& vA, 
                   const Vector3R& vB, 
                   const Vector3R& vC) :
    Primitive(bsdf), 
    _vA(vA),
    _vB(vB), 
    _vC(vC) {

    CADISE_ASSERT(bsdf);

    _eAB = _vB - _vA;
    _eAC = _vC - _vA;

    // TODO: refactor here
    Vector3R faceNormal = _eAB.cross(_eAC);
    faceNormal = (faceNormal.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : faceNormal.normalize();

    _nA = faceNormal;
    _nB = faceNormal;
    _nC = faceNormal;

    _uvwA = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
}

AABB3R Triangle::bound() const {
    return AABB3R(_vA).unionWith(_vB).unionWith(_vC).expand(0.0001_r);
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;
    bool isBackSide = false;

    const Vector3R D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
        isBackSide = true;
    }
    const Vector3R T = ray.origin() - _vA;
    const Vector3R Q = T.cross(eAB);
    const Vector3R P = D.cross(eAC);

    const real denominator = P.dot(eAB);
    if (denominator == 0.0_r) {
        return false;
    }

    const real invDenominator = 1.0_r / denominator;
    const real t = Q.dot(eAC) * invDenominator;
    const real u = P.dot(T) * invDenominator;
    const real v = Q.dot(D) * invDenominator;

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
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;
    bool isBackSide = false;

    const Vector3R D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
        isBackSide = true;
    }
    const Vector3R T = ray.origin() - _vA;
    const Vector3R Q = T.cross(eAB);
    const Vector3R P = D.cross(eAC);

    const real denominator = P.dot(eAB);
    if (denominator == 0.0_r) {
        return false;
    }

    const real invDenominator = 1.0_r / denominator;
    const real t = Q.dot(eAC) * invDenominator;
    const real u = P.dot(T) * invDenominator;
    const real v = Q.dot(D) * invDenominator;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) {
        return false;
    }

    return true;
}

void Triangle::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
    // TODO: refactor here
    Vector3R normal = _eAB.cross(_eAC);
    normal = (normal.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : normal.normalize();
    surfaceInfo.setFrontNormal(normal);

    // TODO: interpolate normal in the triangle
    normal = (primitiveInfo.isBackSide()) ? normal.reverse() : normal;
    surfaceInfo.setGeometryNormal(normal);
    surfaceInfo.setShadingNormal(normal);

    Vector3R uvw;
    if (_textureMapper) {
        _textureMapper->mappingToUvw(surfaceInfo.frontNormal(), &uvw);
        surfaceInfo.setUvw(uvw);
    }
    else {
        /*
             Reference Note:
             https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
        
             which is transcribed from "Real-Time Collision Detection"
        */
        // TODO : integrate with intersecting/occluded 
        //        barycentric coordinate calculation
        const Vector3R v0 = _vB - _vA;
        const Vector3R v1 = _vC - _vA;
        const Vector3R v2 = surfaceInfo.point() - _vA;
        const real d00 = v0.dot(v0);
        const real d01 = v0.dot(v1);
        const real d11 = v1.dot(v1);
        const real d20 = v2.dot(v0);
        const real d21 = v2.dot(v1);
        const real denominator = d00 * d11 - d01 * d01;
        if (denominator == 0.0_r) {
            uvw = Vector3R(0.0_r, 0.0_r, 0.0_r);
        }
        else {
            const real inverseDenominator = 1.0_r / denominator;
            const real u = (d11 * d20 - d01 * d21) * inverseDenominator;
            const real v = (d00 * d21 - d01 * d20) * inverseDenominator;

            uvw = (1.0_r - u - v) * _uvwA +
                  u * _uvwB +
                  v * _uvwC;

            // HACK
            Vector3R n = (1.0_r - u - v) * _nA +
                         u * _nB +
                         v * _nC;
            n = (n.isZero()) ? normal : n;
            surfaceInfo.setShadingNormal(n);
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

    const Vector3R point = _vA + s * eAB + t * eAC;
    const Vector3R direction = point - inSurface.point();
    if (direction.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
    }

    const Vector3R normal = eAB.cross(eAC).normalize();

    outSurface.setPoint(point);
    outSurface.setGeometryNormal(normal);
}

real Triangle::samplePdfA(const Vector3R& position) const {
    return 1.0_r / area();
}

real Triangle::area() const {
    return 0.5_r * _eAB.cross(_eAC).length();
}

void Triangle::setNormalA(const Vector3R& nA) {
    if (!nA.isZero()) {
        _nA = nA;
    }
}

void Triangle::setNormalB(const Vector3R& nB) {
    if (!nB.isZero()) {
        _nB = nB;
    }
}

void Triangle::setNormalC(const Vector3R& nC) {
    if (!nC.isZero()) {
        _nC = nC;
    }
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