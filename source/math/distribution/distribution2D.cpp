#include "math/distribution/distribution2D.h"

#include "fundamental/assertion.h"
#include "math/vector.h"

namespace cadise {

Distribution2D::Distribution2D() = default;

Distribution2D::Distribution2D(const real* const value, const Vector2S& resolution) :
    _conditionalX(resolution.y()) {

    CADISE_ASSERT(value);

    std::vector<real> rowValues(resolution.y());
    for (std::size_t iy = 0; iy < resolution.y(); ++iy) {
        const std::size_t rowIndex = iy * resolution.x();

        for (std::size_t ix = 0; ix < resolution.x(); ++ix) {
            rowValues[iy] += value[rowIndex + ix];
        }

        _conditionalX[iy] = Distribution1D(&(value[rowIndex]), resolution.x());
    }


    _marginalY = Distribution1D(rowValues.data(), resolution.y());
}

Vector2R Distribution2D::sampleContinuous(const Vector2R& seed,
                                          real* const out_pdf) const {
    CADISE_ASSERT(out_pdf);

    real pdfXconditionalY;
    real pdfY;
    std::size_t indexY;

    const real sampleY = _marginalY.sampleContinuous(seed.y(), &pdfY, &indexY);
    const real sampleX = _conditionalX[indexY].sampleContinuous(seed.x(), &pdfXconditionalY);

    *out_pdf = pdfXconditionalY * pdfY;

    return Vector2R(sampleX, sampleY);
}

real Distribution2D::pdfContinuous(const Vector2R& sample) const {
    const std::size_t indexY = _marginalY.continuousToDiscrete(sample.y());

    const real pdfY = _marginalY.pdfContinuous(indexY);
    const real pdfXconditionalY = _conditionalX[indexY].pdfContinuous(sample.x());

    return pdfXconditionalY * pdfY;
}

} // namespace cadise