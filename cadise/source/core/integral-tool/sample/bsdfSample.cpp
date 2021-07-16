#include "core/integral-tool/sample/bsdfSample.h"

#include "fundamental/assertion.h"

namespace cadise {

BsdfSample::BsdfSample() :
    _scatterValue(0.0_r),
    _scatterDirection(0.0_r),
    _pdfW(0.0_r) {
}

const Spectrum& BsdfSample::scatterValue() const {
    return _scatterValue;
}

const Vector3R& BsdfSample::scatterDirection() const {
    return _scatterDirection;
}

real BsdfSample::pdfW() const {
    return _pdfW;
}

void BsdfSample::setScatterValue(const Spectrum& scatterValue) {
    _scatterValue = scatterValue;
}

void BsdfSample::setScatterDirection(const Vector3R& scatterDirection) {
    CADISE_ASSERT(!scatterDirection.isZero());

    _scatterDirection = scatterDirection;
}

void BsdfSample::setPdfW(const real pdfW) {
    _pdfW = pdfW;
}

bool BsdfSample::isValid() const {
    return !_scatterValue.isZero() && _pdfW > 0.0_r;
}

} // namespace cadise