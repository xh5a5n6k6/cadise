#pragma once

#include "core/spectrum/spectrum.h"
#include "math/tVector3.h"

namespace cadise {

class EmitLightSample {
public:
    EmitLightSample();

    const Vector3R& emitPosition() const;
    const Vector3R& emitDirection() const;
    const Vector3R& emitNormal() const;
    const Spectrum& radiance() const;
    real pdfA() const;
    real pdfW() const;

    void setEmitPosition(const Vector3R& emitPosition);
    void setEmitDirection(const Vector3R& emitDirection);
    void setEmitNormal(const Vector3R& emitNormal);
    void setRadiance(const Spectrum& radiance);
    void setPdfA(const real pdfA);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Vector3R _emitPosition;
    Vector3R _emitDirection;
    Vector3R _emitNormal;
    Spectrum _radiance;
    real     _pdfA;
    real     _pdfW;
};

} // namespace cadise