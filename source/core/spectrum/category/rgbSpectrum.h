#pragma once

#include "core/spectrum/conceptualSpectrum.h"

namespace cadise {

class RGBSpectrum : public ConceptualSpectrum<3> {
public:
    RGBSpectrum();
    RGBSpectrum(const real v);
    RGBSpectrum(const Vector3R& v);
    RGBSpectrum(const ConceptualSpectrum<3>& cs);

    void transformIntoRGB(Vector3R& rgb) const;
};

} // namespace cadise