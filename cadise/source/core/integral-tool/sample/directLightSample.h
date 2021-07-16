#pragma once

#include "core/spectrum/spectrum.h"
#include "math/tVector3.h"

namespace cadise {

class DirectLightSample {
public:
    DirectLightSample();

    const Vector3R& targetPosition() const;
    const Vector3R& emitPosition() const;
    const Vector3R& emitNormal() const;
    const Spectrum& radiance() const;
    real pdfW() const;

    void setTargetPosition(const Vector3R& targetPosition);
    void setEmitPosition(const Vector3R& emitPosition);
    void setEmitNormal(const Vector3R& emitNormal);
    void setRadiance(const Spectrum& radiance);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Vector3R _targetPosition;
    Vector3R _emitPosition;
    Vector3R _emitNormal;
    Spectrum _radiance;
    real     _pdfW;
};

} // namespace cadise