#include "core/intersector/primitive/category/triangle.h"

#include "core/intersector/primitive/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/textureMapper.h"

#include "math/constant.h"

#include <limits>
#include <random>

namespace cadise {

Triangle::Triangle(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& v1, const Vector3R& v2, const Vector3R& v3) :
    Primitive(bsdf), 
    _v1(v1),
    _v2(v2), 
    _v3(v3) {
    
    _e1 = _v2 - _v1;
    _e2 = _v3 - _v1;
}

AABB3R Triangle::bound() const {
    return AABB3R(_v1).unionWith(_v2).unionWith(_v3).expand(0.0001_r);
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R D = ray.direction();
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    bool isBackSide = false;
    if (D.dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
        isBackSide = true;
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
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isBackSide);

    return true;
}

bool Triangle::isOccluded(Ray& ray) const {
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

    return true;
}

void Triangle::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
    Vector3R normal = _e1.cross(_e2).normalize();
    surfaceInfo.setFrontNormal(normal);

    normal = (primitiveInfo.isBackSide()) ? normal.composite() : normal;
    surfaceInfo.setGeometryNormal(normal);
    surfaceInfo.setShadingNormal(normal);

    Vector3R uvw;
    if (_textureMapper != nullptr) {
        uvw = _textureMapper->mappingToUvw(surfaceInfo);
        surfaceInfo.setUvw(uvw);
    }
    else {
        
    }
}

void Triangle::sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const {
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

    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    Vector3R point = _v1 + s * e1 + t * e2;
    Vector3R direction = point - inSurface.point();
    if (direction.dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
    }
    Vector3R normal = e1.cross(e2).normalize();

    outSurface.setPoint(point);
    outSurface.setGeometryNormal(normal);
}

real Triangle::samplePdfA(const Vector3R& position) const {
    assert(area() > 0.0_r);

    return 1.0_r / area();
}

real Triangle::area() const {
    return 0.5_r * _e1.cross(_e2).length();
}

} // namespace cadise