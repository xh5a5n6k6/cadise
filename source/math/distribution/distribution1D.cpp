#include "math/distribution/distribution1D.h"

#include "fundamental/assertion.h"

namespace cadise {

Distribution1D::Distribution1D() = default;

Distribution1D::Distribution1D(const real* const value, const std::size_t sizeNumber) :
    _cdf(sizeNumber + 1) {

    CADISE_ASSERT(value);

    _delta = 1.0_r / static_cast<real>(sizeNumber);

    _cdf[0] = 0.0_r;
    for (std::size_t i = 1; i <= sizeNumber; ++i) {
        _cdf[i] = _cdf[i - 1] + value[i - 1] * _delta;
    }

    const real valueSum = _cdf.back();
    
    // normalize cdf
    if (valueSum > 0.0_r) {
        const real inverseValueSum = 1.0_r / valueSum;

        for (std::size_t i = 1; i <= sizeNumber; ++i) {
            _cdf[i] *= inverseValueSum;
        }
    }
    else {
        // use linear cdf instead
        for (std::size_t i = 1; i <= sizeNumber; ++i) {
            _cdf[i] = static_cast<real>(i) * _delta;
        }
    }

    // ensure cdf's last value is 1
    _cdf[sizeNumber] = 1.0_r;
}

real Distribution1D::sampleContinuous(const real seed) const {
    return sampleContinuous(seed, nullptr, nullptr);
}

real Distribution1D::sampleContinuous(const real seed, 
                                      real* const out_pdf) const {
    CADISE_ASSERT(out_pdf);

    return sampleContinuous(seed, out_pdf, nullptr);
}

real Distribution1D::sampleContinuous(const real seed, 
                                      real* const out_pdf, 
                                      std::size_t* const out_index) const {

    const std::size_t sampleIndex = continuousToDiscrete(seed);
    if (out_index) {
        *out_index = sampleIndex;
    }

    real deltaValue = seed - _cdf[sampleIndex];
    if (_cdf[sampleIndex + 1] > _cdf[sampleIndex]) {
        deltaValue /= _cdf[sampleIndex + 1] - _cdf[sampleIndex];
    }

    if (out_pdf) {
        *out_pdf = pdfContinuous(sampleIndex);
    }

    return (static_cast<real>(sampleIndex) + deltaValue) * _delta;
}

real Distribution1D::pdfContinuous(const real sample) const {
    const std::size_t sampleIndex = continuousToDiscrete(sample);

    return pdfContinuous(sampleIndex);
}

real Distribution1D::pdfContinuous(const std::size_t sampleIndex) const {
    return (_cdf[sampleIndex + 1] - _cdf[sampleIndex]) / _delta;
}

std::size_t Distribution1D::continuousToDiscrete(const real seed) const {
    for (std::size_t i = _cdf.size() - 1; i >= 0; --i) {
        if (_cdf[i] <= seed) {
            return i;
        }
    }

    return 0;
}

} // namespace cadise