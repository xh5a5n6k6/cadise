#include "core/renderer/photon-mapping/pmViewpoint.h"

#include "fundamental/assertion.h"

namespace cadise 
{

PmViewpoint::PmViewpoint() :
    _bsdf(nullptr),
    _surfaceDetail(),
    _fromDirection(),
    _filmPosition(),
    _throughputImportance(),
    _emittedRadiance(),
    _numPhotons(),
    _radius(),
    _tau() 
{}

PmViewpoint::PmViewpoint(
    const Bsdf* const    bsdf,
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

const Bsdf* PmViewpoint::bsdf() const
{
    return _bsdf;
}

const SurfaceDetail& PmViewpoint::surfaceDetail() const 
{
    return _surfaceDetail;
}

const Vector3R& PmViewpoint::fromDirection() const 
{
    return _fromDirection;
}

const Vector2D& PmViewpoint::filmPosition() const
{
    return _filmPosition;
}

const Spectrum& PmViewpoint::throughputImportance() const 
{
    return _throughputImportance;
}

const Spectrum& PmViewpoint::emittedRadiance() const 
{
    return _emittedRadiance;
}

const real PmViewpoint::numPhotons() const
{
    return _numPhotons;
}

const real PmViewpoint::radius() const
{
    return _radius;
}

const Spectrum& PmViewpoint::tau() const 
{
    return _tau;
}

void PmViewpoint::setThroughputImportance(const Spectrum& throughputImportance) 
{
    _throughputImportance = throughputImportance;
}

void PmViewpoint::setEmittedRadiance(const Spectrum& emittedRadiance) 
{
    _emittedRadiance = emittedRadiance;
}

void PmViewpoint::setNumPhotons(const real numPhotons)
{
    _numPhotons = numPhotons;
}

void PmViewpoint::setRadius(const real radius) 
{
    _radius = radius;
}

void PmViewpoint::setTau(const Spectrum& tau) 
{
    _tau = tau;
}

} // namespace cadise