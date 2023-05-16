#pragma once

#include "Core/Spectrum/Spectrum.h"
#include "Math/TVector3.h"

namespace cadise
{

class BSDFSample
{
public:
    BSDFSample();

    const Spectrum& scatterValue() const;
    const Vector3R& scatterDirection() const;
    real pdfW() const;

    void setScatterValue(const Spectrum& scatterValue);
    void setScatterDirection(const Vector3R& scatterDirection);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Spectrum _scatterValue;
    Vector3R _scatterDirection;
    real     _pdfW;
};

} // namespace cadise