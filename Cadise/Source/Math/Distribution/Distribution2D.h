#pragma once

#include "math/distribution/distribution1D.h"
#include "math/type/mathType.h"

#include <array>

namespace cadise 
{

class Distribution2D 
{
public:
    Distribution2D();
    Distribution2D(const real* const value, const Vector2S& resolution);

    Vector2R sampleContinuous(
        const std::array<real, 2>& seed,
        real* const                out_pdf) const;

    real pdfContinuous(const std::array<real, 2>& sample) const;

private:
    Distribution1D              _marginalY;
    std::vector<Distribution1D> _conditionalX;
};

} // namespace cadise