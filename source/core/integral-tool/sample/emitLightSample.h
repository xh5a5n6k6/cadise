#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class EmitLightSample {
public:
    EmitLightSample();

    const Vector3R& emitPosition() const;
    const Spectrum& radiance() const;
    real pdfA() const;

    void setEmitPosition(const Vector3R& emitPosition);
    void setRadiance(const Spectrum& radiance);
    void setPdfA(const real pdfA);

    bool isValid() const;

private:
    Vector3R _emitPosition;
    Vector3R _emitDirection;
    Spectrum _radiance;
    real     _pdfA;
    real     _pdfW;
};

} // namespace cadise