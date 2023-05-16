#include "Core/Renderer/PhotonMapping/PMViewpoint.h"

#include "Foundation/Assertion.h"

namespace cadise
{

PMViewpoint::PMViewpoint() :
    _bsdf(nullptr),
    _surfaceDetail(),
    _fromDirection(),
    _filmPosition(),
    _throughputImportance(),
    _emittedRadiance(),
    _numPhotons(),
    _radius(),
    _tau()
{
    // TODO: assert bsdf not null
}

PMViewpoint::PMViewpoint(
    const BSDF* const    bsdf,
    const SurfaceDetail& surfaceDetail,
    const Vector3R&      fromDirection,
    const Vector2D&      filmPosition,
    const Spectrum&      throughputImportance,
    const real           radius) :

    _bsdf(bsdf),
    _surfaceDetail(surfaceDetail),
    _fromDirection(fromDirection),
    _filmPosition(filmPosition),
    _throughputImportance(throughputImportance),
    _emittedRadiance(0.0_r),
    _numPhotons(0.0_r),
    _radius(radius),
    _tau(0.0_r)
{}

const BSDF* PMViewpoint::bsdf() const
{
    return _bsdf;
}

const SurfaceDetail& PMViewpoint::surfaceDetail() const
{
    return _surfaceDetail;
}

const Vector3R& PMViewpoint::fromDirection() const
{
    return _fromDirection;
}

const Vector2D& PMViewpoint::filmPosition() const
{
    return _filmPosition;
}

const Spectrum& PMViewpoint::throughputImportance() const
{
    return _throughputImportance;
}

const Spectrum& PMViewpoint::emittedRadiance() const
{
    return _emittedRadiance;
}

real PMViewpoint::numPhotons() const
{
    return _numPhotons;
}

real PMViewpoint::radius() const
{
    return _radius;
}

const Spectrum& PMViewpoint::tau() const
{
    return _tau;
}

void PMViewpoint::setThroughputImportance(const Spectrum& throughputImportance)
{
    _throughputImportance = throughputImportance;
}

void PMViewpoint::setEmittedRadiance(const Spectrum& emittedRadiance)
{
    _emittedRadiance = emittedRadiance;
}

void PMViewpoint::setNumPhotons(const real numPhotons)
{
    _numPhotons = numPhotons;
}

void PMViewpoint::setRadius(const real radius)
{
    _radius = radius;
}

void PMViewpoint::setTau(const Spectrum& tau)
{
    _tau = tau;
}

} // namespace cadise