#include "core/intersector/primitive/category/rectangle.h"

#include "core/intersector/primitive/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceGeometryInfo.h"

#include <limits>
#include <random>

namespace cadise {

Rectangle::Rectangle(const std::shared_ptr<BSDF>& bsdf, const Vector3R& v1, const Vector3R& v2, const Vector3R& v3) :
    Primitive(bsdf),
    _v1(v1),
    _v2(v2),
    _v3(v3) {
    
    _e1 = _v1 - _v2;
    _e2 = _v3 - _v2;
}

AABB3R Rectangle::bound() const {
    return AABB3R(_v1).unionWith(_v2).unionWith(_v3).unionWith(_v2 + _e1 + _e2).expand(0.0001_r);
}

bool Rectangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    bool isBackSide = false;
    if (ray.direction().dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
        isBackSide = true;
    }
    Vector3R normal = e1.cross(e2).normalize();

    real t = (normal.dot(_v2) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0_r || t > ray.maxT()) {
        return false;
    }

    Vector3R vectorOnPlane = ray.at(t) - _v2;
    real projection1 = vectorOnPlane.dot(_e1.normalize());
    real projection2 = vectorOnPlane.dot(_e2.normalize());
    if (projection1 < 0.0_r || projection1 > _e1.length() ||
        projection2 < 0.0_r || projection2 > _e2.length()) {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);
    primitiveInfo.setIsBackSide(isBackSide);

    return true;
}

bool Rectangle::isOccluded(Ray& ray) const {
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    if (ray.direction().dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
    }
    Vector3R normal = e1.cross(e2).normalize();

    real t = (normal.dot(_v2) - normal.dot(ray.origin())) / normal.dot(ray.direction());
    if (t < 0.0_r || t > ray.maxT()) {
        return false;
    }

    Vector3R vectorOnPlane = ray.at(t) - _v2;
    real projection1 = vectorOnPlane.dot(_e1.normalize());
    real projection2 = vectorOnPlane.dot(_e2.normalize());
    if (projection1 < 0.0_r || projection1 > _e1.length() ||
        projection2 < 0.0_r || projection2 > _e2.length()) {
        return false;
    }

    return true;
}

void Rectangle::evaluateGeometryDetail(const PrimitiveInfo& primitiveInfo, SurfaceGeometryInfo& geometryInfo) const {
    Vector3R normal = (primitiveInfo.isBackSide()) ? _e2.cross(_e1) : _e1.cross(_e2);
    normal = normal.normalize();
    geometryInfo.setNormal(normal);
}

void Rectangle::evaluteShadingDetail(SurfaceShadingInfo& shadingInfo) const {

}

void Rectangle::sampleSurface(const SurfaceGeometryInfo& inSurface, SurfaceGeometryInfo& outSurface) const {
    Vector3R e1 = _e1;
    Vector3R e2 = _e2;
    if (e1.length() < e2.length()) {
        e1.swap(e2);
    }

    real longWidth = e1.length();
    real shortWidth = e2.length();

    // TODO
    // improve sample point on rectangle
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<real> disS(0.0_r, 1.0_r);
    std::uniform_real_distribution<real> disT(0.0_r, 1.0_r);
    real s;
    real t;

    // use rejection method
    do {
        s = disS(gen);
        t = disT(gen);
    } while (t > shortWidth / longWidth);

    Vector3R point = _v2 + s * e1 + t * e1.length() * e2.normalize();
    Vector3R direction = point - inSurface.point();
    if (direction.dot(e1.cross(e2)) > 0.0_r) {
        e1.swap(e2);
    }
    Vector3R normal = e1.cross(e2).normalize();

    outSurface.setPoint(point);
    outSurface.setNormal(normal);
}

real Rectangle::samplePdfA(const Vector3R& position) const {
    assert(area() > 0.0_r);

    return 1.0_r / area();
}

real Rectangle::area() const {
    return _e1.length() * _e2.length();
}

} // namespace cadise