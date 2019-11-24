#pragma once

#include "math/distribution/distribution1D.h"

#include "math/type/mathType.h"

namespace cadise {

class Distribution2D {
public:
    Distribution2D();
    Distribution2D(const real* const value, const Vector2S& resolution);

    Vector2R sampleContinuous(const Vector2R& seed,
                              real* const out_pdf) const;

    real pdfContinuous(const Vector2R& sample) const;

private:
    Distribution1D _marginalY;
    std::vector<Distribution1D> _conditionalX;
};

} // namespace cadise