#include "Math/Distribution/Distribution1D.h"

#include "Foundation/Assertion.h"
#include "Math/Math.h"

namespace cadise
{

Distribution1D::Distribution1D() = default;

Distribution1D::Distribution1D(const real* const value, const std::size_t sizeNumber) :
    _cdf(sizeNumber + 1)
{
    CS_ASSERT(value);

    _delta = 1.0_r / static_cast<real>(sizeNumber);

    _cdf[0] = 0.0_r;
    for (std::size_t i = 1; i <= sizeNumber; ++i)
    {
        _cdf[i] = _cdf[i - 1] + value[i - 1] * _delta;
    }

    const real valueSum = _cdf.back();

    // normalize cdf
    if (valueSum > 0.0_r)
    {
        const real rcpValueSum = 1.0_r / valueSum;

        for (std::size_t i = 1; i <= sizeNumber; ++i)
        {
            _cdf[i] *= rcpValueSum;
        }
    }
    else
    {
        // use linear cdf instead
        for (std::size_t i = 1; i <= sizeNumber; ++i)
        {
            _cdf[i] = static_cast<real>(i) * _delta;
        }
    }

    // ensure cdf's last value is 1
    _cdf[sizeNumber] = 1.0_r;
}

real Distribution1D::sampleContinuous(const real seed) const
{
    real localPdf;
    std::size_t localIndex;

    return this->sampleContinuous(seed, &localPdf, &localIndex);
}

real Distribution1D::sampleContinuous(
    const real  seed,
    real* const out_pdf) const
{
    CS_ASSERT(out_pdf);

    std::size_t localIndex;

    return this->sampleContinuous(seed, out_pdf, &localIndex);
}

real Distribution1D::sampleContinuous(
    const real         seed,
    real* const        out_pdf,
    std::size_t* const out_index) const
{
    CS_ASSERT(out_pdf);
    CS_ASSERT(out_index);

    const std::size_t sampleIndex = this->continuousToDiscrete(seed);

    CS_ASSERT_RANGE_INCLUSIVE(sampleIndex, 0, _cdf.size() - 2);

    *out_index = sampleIndex;
    *out_pdf   = this->pdfContinuous(sampleIndex);

    real deltaValue = seed - _cdf[sampleIndex];
    if (_cdf[sampleIndex + 1] > _cdf[sampleIndex])
    {
        deltaValue /= _cdf[sampleIndex + 1] - _cdf[sampleIndex];
    }

    const real sample = (static_cast<real>(sampleIndex) + deltaValue) * _delta;

    return math::clamp(sample, 0.0_r, 1.0_r);
}

std::size_t Distribution1D::sampleDiscrete(const real seed) const
{
    real localPdf;

    return this->sampleDiscrete(seed, &localPdf);
}

std::size_t Distribution1D::sampleDiscrete(
    const real  seed,
    real* const out_pdf) const
{
    CS_ASSERT(out_pdf);

    const std::size_t sampleIndex = this->continuousToDiscrete(seed);
    *out_pdf = this->pdfDiscrete(sampleIndex);

    return sampleIndex;
}

real Distribution1D::pdfContinuous(const real sample) const
{
    const std::size_t sampleIndex = continuousToDiscrete(sample);

    return this->pdfContinuous(sampleIndex);
}

real Distribution1D::pdfContinuous(const std::size_t sampleIndex) const
{
    return (_cdf[sampleIndex + 1] - _cdf[sampleIndex]) / _delta;
}

real Distribution1D::pdfDiscrete(const std::size_t sampleIndex) const
{
    CS_ASSERT_RANGE_INCLUSIVE(sampleIndex, 0, _cdf.size() - 2);

    return _cdf[sampleIndex + 1] - _cdf[sampleIndex];
}

std::size_t Distribution1D::continuousToDiscrete(const real seed) const
{
    CS_ASSERT_RANGE_INCLUSIVE(seed, 0.0_r, 1.0_r);

    for (std::size_t i = 0; i < _cdf.size() - 1; ++i)
    {
        if (_cdf[_cdf.size() - 2 - i] <= seed)
        {
            return i;
        }
    }

    return 0;
}

} // namespace cadise