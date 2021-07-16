#pragma once

#include "core/spectrum/spectrum.h"
#include "core/surfaceDetail.h"
#include "math/tVector2.h"

namespace cadise {

class Bsdf;

class PmViewpoint {
public:
    PmViewpoint();
    PmViewpoint(
        const Bsdf* const    bsdf,
        const SurfaceDetail& surfaceDetail,
        const Vector3R&      fromDirection,
        const Vector2D&      filmPosition,
        const Spectrum&      throughputImportance,
        const real           radius);

    const Bsdf* bsdf() const;
    const SurfaceDetail& surfaceDetail() const;
    const Vector3R& fromDirection() const;
    const Vector2D& filmPosition() const;
    const Spectrum& throughputImportance() const;
    const Spectrum& emittedRadiance() const;
    const real numPhotons() const;
    const real radius() const;
    const Spectrum& tau() const;

    void setThroughputImportance(const Spectrum& throughputImportance);
    void setEmittedRadiance(const Spectrum& emittedRadiance);
    void setNumPhotons(const real numPhotons);
    void setRadius(const real radius);
    void setTau(const Spectrum& tau);

private:
    const Bsdf* _bsdf;

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