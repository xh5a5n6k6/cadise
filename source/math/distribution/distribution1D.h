#pragma once

#include "math/type/fundamentalType.h"

#include <vector>

namespace cadise {

class Distribution1D {
public:
    Distribution1D();
    Distribution1D(const real* const value, const std::size_t sizeNumber);

    real sampleContinuous(const real seed) const;
    real sampleContinuous(const real seed,
                          real* const out_pdf) const;
    real sampleContinuous(const real seed, 
                          real* const out_pdf,
                          std::size_t* const out_index) const;
    
    real pdfContinuous(const real sample) const;
    real pdfContinuous(const std::size_t sampleIndex) const;

    std::size_t continuousToDiscrete(const real seed) const;

private:
    real _delta;
    std::vector<real> _cdf;
};

} // namespace cadise