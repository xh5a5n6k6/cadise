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
                   const Vector3R&              vA, 
                   const Vector3R&              vB, 
                   const Vector3R&              vC) :
    Primitive(bsdf), 
    _vA(vA),
    _vB(vB), 
    _vC(vC) {

    CADISE_ASSERT(bsdf);

    _eAB = _vB - _vA;
    _eAC = _vC - _vA;

    // TODO: refactor here
    Vector3R N = _eAB.cross(_eAC);
    N = (N.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : N.normalize();

    _nA = N;
    _nB = N;
    _nC = N;

    _uvwA = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
}

void Triangle::evaluateBound(AABB3R* const out_bound) const {
    CADISE_ASSERT(out_bound);

    AABB3R bound(_vA);
    bound.unionWith(_vB).unionWith(_vC).expand(0.0001_r);

    *out_bound = bound;
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;

    const Vector3R& D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
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

    return true;
}

bool Triangle::isOccluded(const Ray& ray) const {
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;

    const Vector3R& D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) {
        eAB.swap(eAC);
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

void Triangle::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceInfo* const   out_surface) const {
    
    CADISE_ASSERT(out_surface);
    
    // TODO: refactor here
    const Vector3R& P = out_surface->point();
    Vector3R Ng = _eAB.cross(_eAC);
    Ng = (Ng.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : Ng.normalize();

    Vector3R uvw;
    if (_textureMapper) {
        _textureMapper->mappingToUvw(Ng, &uvw);

        out_surface->setUvw(uvw);
    }
    else {
        Vector3R barycentric;
        _positionToBarycentric(P, &barycentric);

        uvw = barycentric.x() * _uvwA +
              barycentric.y() * _uvwB +
              barycentric.z() * _uvwC;

        // HACK
        Vector3R Ns = barycentric.x() * _nA +
                      barycentric.y() * _nB +
                      barycentric.z() * _nC;
        Ns = (Ns.isZero()) ? Ng : Ns.normalize();

        out_surface->setShadingNormal(Ns);
        out_surface->setGeometryNormal(Ng);
        out_surface->setUvw(uvw);
    }
}

void Triangle::sampleSurface(
    const SurfaceInfo& inSurface, 
    SurfaceInfo* const out_surface) const {
    
    CADISE_ASSERT(out_surface);

    // TODO
    // improve sample point on triangle
    real s;
    real t;

    // Use rejection method
    do {
        s = Random::nextReal();
        t = Random::nextReal();
    } while (s + t >= 1.0_r);

    const Vector3R eAB = _eAB;
    const Vector3R eAC = _eAC;
    const Vector3R P   = _vA + s * eAB + t * eAC;

    Vector3R barycentric;
    _positionToBarycentric(P, &barycentric);

    Vector3R Ng = eAB.cross(eAC);
    Ng = (Ng.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : Ng.normalize();

    Vector3R Ns = barycentric.x() * _nA +
                  barycentric.y() * _nB +
                  barycentric.z() * _nC;
    Ns = (Ns.isZero()) ? Ng : Ns;

    out_surface->setPoint(P);
    out_surface->setGeometryNormal(Ng);
    out_surface->setShadingNormal(Ns);
}

real Triangle::samplePdfA(const Vector3R& position) const {
    return 1.0_r / area();
}

real Triangle::area() const {
    return 0.5_r * _eAB.cross(_eAC).length();
}

void Triangle::setNa(const Vector3R& nA) {
    if (!nA.isZero()) {
        _nA = nA;
    }
}

void Triangle::setNb(const Vector3R& nB) {
    if (!nB.isZero()) {
        _nB = nB;
    }
}

void Triangle::setNc(const Vector3R& nC) {
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

void Triangle::_positionToBarycentric(
    const Vector3R& position, 
    Vector3R* const out_barycentric) const {
    
    CADISE_ASSERT(out_barycentric);

    const Vector3R v0 = _vB - _vA;
    const Vector3R v1 = _vC - _vA;
    const Vector3R v2 = position - _vA;

    const real d00 = v0.dot(v0);
    const real d01 = v0.dot(v1);
    const real d11 = v1.dot(v1);
    const real d20 = v2.dot(v0);
    const real d21 = v2.dot(v1);
    const real denominator = d00 * d11 - d01 * d01;

    if (denominator == 0.0_r) {
        *out_barycentric = Vector3R(0.0_r, 0.0_r, 0.0_r);
    }
    else {
        const real inverseDenominator = 1.0_r / denominator;
        const real u = (d11 * d20 - d01 * d21) * inverseDenominator;
        const real v = (d00 * d21 - d01 * d20) * inverseDenominator;

        *out_barycentric = Vector3R(1.0_r - u - v, 
                                    u, 
                                    v);
    }
}

} // namespace cadise