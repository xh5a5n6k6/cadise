#include "core/intersector/primitive/sphere.h"

#include "core/integral-tool/sample/positionSample.h"
#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceInfo.h"
#include "core/texture/mapper/sphericalMapper.h"
#include "fundamental/assertion.h"
#include "math/aabb.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/transform.h"

#include <cmath>

namespace cadise {

Sphere::Sphere(const std::shared_ptr<Bsdf>& bsdf, const Vector3R& center, const real radius) :
    Primitive(bsdf), 
    _center(center),
    _radius(radius) {

    CADISE_ASSERT(bsdf);

    _worldToLocal     = std::make_shared<Transform>(Matrix4R::translate(center.reverse()));
    _tmptextureMapper = std::make_shared<SphericalMapper>();
}

void Sphere::evaluateBound(AABB3R* const out_bound) const {
    CADISE_ASSERT(out_bound);

    AABB3R bound(_center - _radius, _center + _radius);
    bound.expand(0.0001_r);

    *out_bound = bound;
}

bool Sphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const {
    /*
        Reference Note:
        Ray Tracing Gems, p.87 ~ p.94
        "Precision Improvements for Ray/Sphere Intersection"
        Haines et al., 2019
    */
    const Vector3R& O = ray.origin();
    const Vector3R  G = _center;
    const Vector3R  f = O - G;
    const Vector3R& d = ray.direction();
    const real      b = -f.dot(d);
    const real      r = _radius;

    const Vector3R l = f + b * d;
    const real discriminant = r * r - l.absDot(l);
    if (discriminant >= 0.0_r) {
        const real signB = (b > 0.0_r) ? 1.0_r : -1.0_r;
        const real q     = b + signB * std::sqrt(discriminant);
        const real c     = f.absDot(f) - r * r;

        const real t0 = c / q;
        const real t1 = q;
        if (t0 > ray.maxT() || t1 < ray.minT()) {
            return false;
        }

        real t = t0;
        if (t < ray.minT()) {
            t = t1;

            if (t > ray.maxT()) {
                return false;
            }
        }

        ray.setMaxT(t);
        primitiveInfo.setPrimitive(this);

        return true;
    }
    else {
        return false;
    }
}

bool Sphere::isOccluded(const Ray& ray) const {
    const Vector3R& O = ray.origin();
    const Vector3R  G = _center;
    const Vector3R  f = O - G;
    const Vector3R& d = ray.direction();
    const real      b = -f.dot(d);
    const real      r = _radius;

    const Vector3R l = f + b * d;
    const real discriminant = r * r - l.absDot(l);
    if (discriminant >= 0.0_r) {
        const real signB = (b > 0.0_r) ? 1.0_r : -1.0_r;
        const real q     = b + signB * std::sqrt(discriminant);
        const real c     = f.absDot(f) - r * r;

        const real t0 = c / q;
        const real t1 = q;
        if (t0 > ray.maxT() || t1 < ray.minT()) {
            return false;
        }

        real t = t0;
        if (t < ray.minT()) {
            t = t1;

            if (t > ray.maxT()) {
                return false;
            }
        }

        return true;
    }
    else {
        return false;
    }
}

void Sphere::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceInfo* const   out_surface) const {

    CADISE_ASSERT(out_surface);

    const Vector3R& P = out_surface->position();
    const Vector3R  N = (P - _center).normalize();

    out_surface->setGeometryNormal(N);
    out_surface->setShadingNormal(N);

    Vector3R uvw;
    if (_textureMapper) {
        _textureMapper->mappingToUvw(out_surface->shadingNormal(), &uvw);
        out_surface->setUvw(uvw);
    }
    else {
        _tmptextureMapper->mappingToUvw(out_surface->shadingNormal(), &uvw);
        out_surface->setUvw(uvw);
    }
}

void Sphere::evaluatePositionSample(PositionSample* const out_sample) const {
    CADISE_ASSERT(out_sample);

    // TODO: implement here
}

real Sphere::evaluatePositionPdfA(const Vector3R& position) const {
    return 1.0_r / this->area();
}

real Sphere::area() const {
    return constant::four_pi<real> * _radius * _radius;
}

} // namespace cadise