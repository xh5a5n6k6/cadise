#pragma once

#include "Core/Spectrum/Spectrum.h"
#include "Core/SurfaceDetail.h"
#include "Math/TVector2.h"

// forward declaration
namespace cadise
{
    class BSDF;
}

namespace cadise
{

class PMViewpoint
{
public:
    PMViewpoint();
    PMViewpoint(
        const BSDF* const    bsdf,
        const SurfaceDetail& surfaceDetail,
        const Vector3R&      fromDirection,
        const Vector2D&      filmPosition,
        const Spectrum&      throughputImportance,
        const real           radius);

    const BSDF* bsdf() const;
    const SurfaceDetail& surfaceDetail() const;
    const Vector3R& fromDirection() const;
    const Vector2D& filmPosition() const;
    const Spectrum& throughputImportance() const;
    const Spectrum& emittedRadiance() const;
    real numPhotons() const;
    real radius() const;
    const Spectrum& tau() const;

    void setThroughputImportance(const Spectrum& throughputImportance);
    void setEmittedRadiance(const Spectrum& emittedRadiance);
    void setNumPhotons(const real numPhotons);
    void setRadius(const real radius);
    void setTau(const Spectrum& tau);

private:
    const BSDF* _bsdf;

    SurfaceDetail _surfaceDetail;
    Vector3R      _fromDirection;
    Vector2D      _filmPosition;
    Spectrum      _throughputImportance;
    Spectrum      _emittedRadiance;

    real        _numPhotons;
    real        _radius;
    Spectrum    _tau;
};

} // namespace cadise