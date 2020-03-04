#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class CameraSample {
public:
    CameraSample();

    const Vector3R& targetPosition() const;
    const Vector2R& filmNdcPosition() const;
    const Spectrum& importance() const;
    real pdfW() const;

    void setTargetPosition(const Vector3R& targetPosition);
    void setFilmNdcPosition(const Vector2R& filmNdcPosition);
    void setImportance(const Spectrum& importance);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Vector3R _targetPosition;
    Vector2R _filmNdcPosition;
    Spectrum _importance;
    real     _pdfW;
};

} // namespace cadise