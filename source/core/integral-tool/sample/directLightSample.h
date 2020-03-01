#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class DirectLightSample {
public:
    DirectLightSample();

    const Vector3R& emitPosition() const;
    const Vector3R& targetPosition() const;
    const Spectrum& radiance() const;
    real pdfW() const;

    void setEmitPosition(const Vector3R& emitPosition);
    void setTargetPosition(const Vector3R& targetPosition);
    void setRadiance(const Spectrum& radiance);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Vector3R _emitPosition;
    Vector3R _targetPosition;
    Spectrum _radiance;
    real     _pdfW;
};

} // namespace cadise