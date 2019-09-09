#include "core/intersector/primitive/category/rectangle.h"

#include "core/intersector/primitive/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/textureMapper.h"

#include "fundamental/assertion.h"

#include "math/random.h"

#include <limits>

namespace cadise {

Rectangle::Rectangle(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& vA, const Vector3R& vB, const Vector3R& vC) :
    Primitive(bsdf),
    _vA(vA),
    _vB(vB),
    _vC(vC) {
    
    _eA = _vA - _vB;
    _eB = _vC - _vB;

    _vD = _vB + _eA + _eB;
    
    _uvwA = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
    _uvwD = Vector3R(1.0_r, 1.0_r, 0.0_r);
}

AABB3R Rectangle::bound() const {
    return AABB3R(_vA).unionWith(_vB).unionWith(_vC).unionWith(_vD).expand(0.0001_r);
}

bool Rectangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    bool isBackSide = false;
    if (ray.direction().dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
        isBackSide = true;
    }
    Vector3R normal = eA.cross(eB).normalize();

    real t = (normal.dot(_vB) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0_r || t > ray.maxT()) {
        return false;
    }

    Vector3R vectorOnPlane = ray.at(t) - _vB;
    real projection1 = vectorOnPlane.dot(_eA.normalize());
    real projection2 = vectorOnPlane.dot(_eB.normalize());
    if (projection1 < 0.0_r || projection1 > _eA.length() ||
        projection2 < 0.0_r || projection2 > _eB.length()) {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isBackSide);

    return true;
}

bool Rectangle::isOccluded(Ray& ray) const {
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    if (ray.direction().dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
    }
    Vector3R normal = eA.cross(eB).normalize();

    real t = (normal.dot(_vB) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0_r || t > ray.maxT()) {
        return false;
    }

    Vector3R vectorOnPlane = ray.at(t) - _vB;
    real projection1 = vectorOnPlane.dot(_eA.normalize());
    real projection2 = vectorOnPlane.dot(_eB.normalize());
    if (projection1 < 0.0_r || projection1 > _eA.length() ||
        projection2 < 0.0_r || projection2 > _eB.length()) {
        return false;
    }

    return true;
}

void Rectangle::evaluateSurfaceDetail(const PrimitiveInfo& primitiveInfo, SurfaceInfo& surfaceInfo) const {
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
        Vector3R point = surfaceInfo.point();
        Vector3R vectorOnPlane = point - _vB;
        real projection1 = vectorOnPlane.dot(_eA.normalize()) / _eA.length();
        real projection2 = vectorOnPlane.dot(_eB.normalize()) / _eA.length();

        Vector3R xUvwLerp1 = _uvwB.lerp(_uvwC, projection2);
        Vector3R xUvwLerp2 = _uvwA.lerp(_uvwD, projection2);
        uvw = xUvwLerp1.lerp(xUvwLerp2, projection1);
        surfaceInfo.setUvw(uvw);
    }
}

void Rectangle::sampleSurface(const SurfaceInfo& inSurface, SurfaceInfo& outSurface) const {
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    outSurface.setFrontNormal(eA.cross(eB).normalize());

    if (eA.length() < eB.length()) {
        eA.swap(eB);
    }

    real longWidth = eA.length();
    real shortWidth = eB.length();

    // TODO
    // improve sample point on rectangle
    real s;
    real t;

    // use rejection method
    do {
        s = random::get1D();
        t = random::get1D();
    } while (t > shortWidth / longWidth);

    Vector3R point = _vB + s * eA + t * eA.length() * eB.normalize();
    Vector3R direction = point - inSurface.point();
    if (direction.dot(eA.cross(eB)) > 0.0_r) {
        eA.swap(eB);
    }
    Vector3R normal = eA.cross(eB).normalize();

    outSurface.setPoint(point);
    outSurface.setGeometryNormal(normal);
}

real Rectangle::samplePdfA(const Vector3R& position) const {
    CADISE_ASSERT(area() > 0.0_r);

    return 1.0_r / area();
}

real Rectangle::area() const {
    return _eA.length() * _eB.length();
}

void Rectangle::setUvwA(const Vector3R& uvwA) {
    _uvwA = uvwA;
}

void Rectangle::setUvwB(const Vector3R& uvwB) {
    _uvwB = uvwB;
}

void Rectangle::setUvwC(const Vector3R& uvwC) {
    _uvwC = uvwC;
}

void Rectangle::setUvwD(const Vector3R& uvwD) {
    _uvwD = uvwD;
}

} // namespace cadise