#include "core/intersector/primitive/sphere.h"

#include "core/integral-tool/sample/positionSample.h"
#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceDetail.h"
#include "core/texture/mapper/sphericalMapper.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/tAabb3.h"
#include "math/transform.h"

#include <cmath>

namespace cadise 
{

Sphere::Sphere(
    const std::shared_ptr<Bsdf>& bsdf, 
    const Vector3R&              center, 
    const real                   radius) :
    
    Primitive(bsdf), 
    _center(center),
    _radius(radius) 
{
    CS_ASSERT(bsdf);

    _worldToLocal     = std::make_shared<Transform>(Matrix4R::makeTranslate(center.negate()));
    _tmptextureMapper = std::make_shared<SphericalMapper>();
}

void Sphere::evaluateBound(AABB3R* const out_bound) const 
{
    CS_ASSERT(out_bound);

    AABB3R bound(_center.sub(_radius), _center.add(_radius));
    bound.expandLocal(0.0001_r);

    out_bound->set(bound);
}

bool Sphere::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const
{
    /*
        Reference Note:
        Ray Tracing Gems, p.87 ~ p.94
        "Precision Improvements for Ray/Sphere Intersection"
        Haines et al., 2019
    */
    const Vector3R& O = ray.origin();
    const Vector3R  G = _center;
    const Vector3R  f = O.sub(G);
    const Vector3R& d = ray.direction();
    const real      b = f.negate().dot(d);
    const real      r = _radius;

    const Vector3R l = f.add(d.mul(b));
    const real discriminant = r * r - l.absDot(l);
    if (discriminant < 0.0_r)
    {
        return false;
    }

    const real signB = (b > 0.0_r) ? 1.0_r : -1.0_r;
    const real q     = b + signB * std::sqrt(discriminant);
    const real c     = f.absDot(f) - r * r;

    const real t0 = c / q;
    const real t1 = q;

    real t;
    if (!_isSolutionValid(t0, t1, ray.minT(), ray.maxT(), &t)) 
    {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);

    return true;
}

bool Sphere::isOccluded(const Ray& ray) const 
{
    const Vector3R& O = ray.origin();
    const Vector3R  G = _center;
    const Vector3R  f = O.sub(G);
    const Vector3R& d = ray.direction();
    const real      b = f.negate().dot(d);
    const real      r = _radius;

    const Vector3R l = f.add(d.mul(b));
    const real discriminant = r * r - l.absDot(l);
    if (discriminant < 0.0_r)
    {
        return false;
    }

    const real signB = (b > 0.0_r) ? 1.0_r : -1.0_r;
    const real q     = b + signB * std::sqrt(discriminant);
    const real c     = f.absDot(f) - r * r;

    const real t0 = c / q;
    const real t1 = q;

    real t;
    if (!_isSolutionValid(t0, t1, ray.minT(), ray.maxT(), &t))
    {
        return false;
    }

    return true;
}

void Sphere::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceDetail* const out_surface) const 
{
    CS_ASSERT(out_surface);

    const Vector3R& P       = out_surface->position();
    const Vector3R  NVector = P.sub(_center);
    const Vector3R  N       = NVector.normalize();

    out_surface->setGeometryNormal(N);
    out_surface->setShadingNormal(N);

    Vector3R uvw;
    if (_textureMapper)
    {
        _textureMapper->mappingToUvw(out_surface->shadingNormal(), &uvw);
        out_surface->setUvw(uvw);
    }
    else 
    {
        _tmptextureMapper->mappingToUvw(out_surface->shadingNormal(), &uvw);
        out_surface->setUvw(uvw);
    }

    /*
        calculate differential geometry properties.
        
        implementation follows PBRT-v3's solution
        Reference: PBRT-v3 e-book (Sphere)
    */
    Vector3R dPdU;
    Vector3R dPdV;
    Vector3R dNdU;
    Vector3R dNdV;
    {
        const real xzProjection = std::max(_radius * _radius - NVector.y() * NVector.y(), 0.0_r);
        const real rSinTheta    = std::sqrt(xzProjection);
        const real cosPhi       = (rSinTheta != 0.0_r) ? NVector.z() / rSinTheta : 1.0_r;
        const real sinPhi       = (rSinTheta != 0.0_r) ? NVector.x() / rSinTheta : 0.0_r;

        dPdU = Vector3R(
            constant::two_pi<real> * NVector.z(),
            0.0_r,
            -1.0_r * constant::two_pi<real> * NVector.x());

        dPdV = Vector3R(
            -1.0_r * NVector.y() * sinPhi,
            rSinTheta,
            -1.0_r * NVector.y() * cosPhi).mul(constant::pi<real>);

        const Vector3R d2PdUU = Vector3R(
            NVector.x(),
            0.0_r,
            NVector.z()).mul(-1.0_r * constant::two_pi<real> * constant::two_pi<real>);

        const Vector3R d2PdUV = Vector3R(
            -cosPhi,
            0.0_r,
            sinPhi).mul(constant::pi<real> * NVector.y() * constant::two_pi<real>);

        const Vector3R d2PdVV = Vector3R(
            NVector.x(),
            NVector.y(),
            NVector.z()).mul(-1.0_r * constant::pi<real> * constant::pi<real>);

        const real E = dPdU.dot(dPdU);
        const real F = dPdU.dot(dPdV);
        const real G = dPdV.dot(dPdV);
        const real e = N.dot(d2PdUU);
        const real f = N.dot(d2PdUV);
        const real g = N.dot(d2PdVV);

        const real EGsubF2     = E * G - F * F;
        const real inverseEGF2 = (EGsubF2 != 0.0_r) ? 1.0_r / EGsubF2 : 1.0_r;

        dNdU = dPdU.mul((f * F - e * G) * inverseEGF2).add(
               dPdV.mul((e * F - f * E) * inverseEGF2));
        dNdV = dPdU.mul((g * F - f * G) * inverseEGF2).add(
               dPdV.mul((f * F - g * E) * inverseEGF2));
    }

    out_surface->setDifferentialGeometry({ dPdU, dPdV, dNdU, dNdV });
}

void Sphere::evaluatePositionSample(PositionSample* const out_sample) const
{
    CS_ASSERT(out_sample);

    // TODO: implement here
}

real Sphere::evaluatePositionPdfA(const Vector3R& position) const
{
    return 1.0_r / this->area();
}

real Sphere::area() const
{
    return constant::four_pi<real> * _radius * _radius;
}

bool Sphere::_isSolutionValid(
    const real  t0,
    const real  t1,
    const real  minT,
    const real  maxT,
    real* const out_finalT) const
{
    CS_ASSERT(out_finalT);

    if (t0 > maxT || t1 < minT)
    {
        return false;
    }

    real localT = t0;
    if (localT < minT) 
    {
        localT = t1;

        if (localT > maxT)
        {
            return false;
        }
    }

    *out_finalT = localT;

    return true;
}

} // namespace cadise