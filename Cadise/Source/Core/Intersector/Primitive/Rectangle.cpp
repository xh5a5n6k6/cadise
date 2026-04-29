#include "Core/Intersector/Primitive/Rectangle.h"

#include "Core/Gear/Sample/PositionSample.h"
#include "Core/Intersector/PrimitiveInfo.h"
#include "Core/Ray.h"
#include "Core/SurfaceDetail.h"
#include "Core/Texture/Mapper/TextureMapper.h"
#include "Foundation/Assertion.h"
#include "Math/Random.h"
#include "Math/TAabb3.h"

#include <limits>

namespace cadise
{

Rectangle::Rectangle(
    const std::shared_ptr<BSDF>& bsdf,
    const Vector3R&              vA,
    const Vector3R&              vB,
    const Vector3R&              vC) :

    Primitive(bsdf),
    _vA(vA),
    _vB(vB),
    _vC(vC)
{
    CS_ASSERT(bsdf);

    _eA = _vA.sub(_vB);
    _eB = _vC.sub(_vB);

    _vD = _vB.add(_eA).add(_eB);

    _uvwA = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
    _uvwD = Vector3R(1.0_r, 1.0_r, 0.0_r);
}

void Rectangle::evaluateBound(AABB3R* const out_bound) const
{
    CS_ASSERT(out_bound);

    AABB3R bound(_vA);
    bound.unionWithLocal(_vB).unionWithLocal(_vC).unionWithLocal(_vD).expandLocal(0.0001_r);

    out_bound->set(bound);
}

bool Rectangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const
{
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    if (ray.direction().dot(eA.cross(eB)) > 0.0_r)
    {
        eA.swap(eB);
    }

    const Vector3R normal = eA.cross(eB).normalize();
    const real     t      = normal.dot(_vB.sub(ray.origin())) / normal.dot(ray.direction());
    if (t < ray.minT() || t > ray.maxT())
    {
        return false;
    }

    const Vector3R vectorOnPlane = ray.at(t).sub(_vB);
    const real     projectionA   = vectorOnPlane.dot(_eA.normalize());
    const real     projectionB   = vectorOnPlane.dot(_eB.normalize());
    if (projectionA < 0.0_r || projectionA > _eA.length() ||
        projectionB < 0.0_r || projectionB > _eB.length())
    {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);

    return true;
}

bool Rectangle::isOccluded(const Ray& ray) const
{
    Vector3R eA = _eA;
    Vector3R eB = _eB;
    if (ray.direction().dot(eA.cross(eB)) > 0.0_r)
    {
        eA.swap(eB);
    }

    const Vector3R normal = eA.cross(eB).normalize();
    const real     t      = normal.dot(_vB.sub(ray.origin())) / normal.dot(ray.direction());
    if (t < ray.minT() || t > ray.maxT())
    {
        return false;
    }

    const Vector3R vectorOnPlane = ray.at(t).sub(_vB);
    const real     projectionA   = vectorOnPlane.dot(_eA.normalize());
    const real     projectionB   = vectorOnPlane.dot(_eB.normalize());
    if (projectionA < 0.0_r || projectionA > _eA.length() ||
        projectionB < 0.0_r || projectionB > _eB.length())
    {
        return false;
    }

    return true;
}

void Rectangle::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo,
    SurfaceDetail* const out_surface) const
{
    CS_ASSERT(out_surface);

    Vector3R N = _eA.cross(_eB);
    N = (N.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : N.normalize();

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
        const Vector3R& P             = out_surface->position();
        const Vector3R  vectorOnPlane = P.sub(_vB);
        const real      projectionA   = vectorOnPlane.dot(_eA.normalize()) / _eA.length();
        const real      projectionB   = vectorOnPlane.dot(_eB.normalize()) / _eB.length();

        const Vector3R xUvwLerpA = _uvwB.lerp(_uvwC, projectionB);
        const Vector3R xUvwLerpB = _uvwA.lerp(_uvwD, projectionB);

        uvw = xUvwLerpA.lerp(xUvwLerpB, projectionA);

        out_surface->setUvw(uvw);
    }
}

void Rectangle::evaluatePositionSample(PositionSample* const out_sample) const
{
    CS_ASSERT(out_sample);

    Vector3R eA = _eA;
    Vector3R eB = _eB;

    if (eA.length() < eB.length())
    {
        eA.swap(eB);
    }

    const real longWidth  = eA.length();
    const real shortWidth = eB.length();

    // TODO
    // improve sample point on rectangle
    real s;
    real t;

    // use rejection method
    do
    {
        s = Random::nextReal();
        t = Random::nextReal();
    } while (t > shortWidth / longWidth);

    const Vector3R P = _vB.add(eA.mul(s)).add(eB.normalize().mul(t * eA.length()));

    Vector3R N = _eA.cross(_eB);
    N = (N.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : N.normalize();

    out_sample->setPosition(P);
    out_sample->setGeometryNormal(N);
    out_sample->setShadingNormal(N);
    // TODO: rectangle uvw calculation
    // out_sample->setUvw(uvw);
    out_sample->setPdfA(this->evaluatePositionPdfA(P));
}

real Rectangle::evaluatePositionPdfA(const Vector3R& position) const
{
    return 1.0_r / this->area();
}

real Rectangle::area() const
{
    return _eA.length() * _eB.length();
}

void Rectangle::setUvwA(const Vector3R& uvwA)
{
    _uvwA = uvwA;
}

void Rectangle::setUvwB(const Vector3R& uvwB)
{
    _uvwB = uvwB;
}

void Rectangle::setUvwC(const Vector3R& uvwC)
{
    _uvwC = uvwC;
}

void Rectangle::setUvwD(const Vector3R& uvwD)
{
    _uvwD = uvwD;
}

} // namespace cadise