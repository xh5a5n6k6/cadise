#include "core/integral-tool/sample/cameraSample.h"

#include <cmath>

namespace cadise {

CameraSample::CameraSample() :
    _targetPosition(),
    _filmNdcPosition(),
    _importance(0.0_r),
    _pdfW(0.0_r) {
}

const Vector3R& CameraSample::targetPosition() const {
    return _targetPosition;
}

const Vector2R& CameraSample::filmNdcPosition() const {
    return _filmNdcPosition;
}

const Spectrum& CameraSample::importance() const {
    return _importance;
}

real CameraSample::pdfW() const {
    return _pdfW;
}

void CameraSample::setTargetPosition(const Vector3R& targetPosition) {
    _targetPosition = targetPosition;
}

void CameraSample::setFilmNdcPosition(const Vector2R& filmNdcPosition) {
    _filmNdcPosition = filmNdcPosition;
}

void CameraSample::setImportance(const Spectrum& importance) {
    _importance = importance;
}

void CameraSample::setPdfW(const real pdfW) {
    _pdfW = pdfW;
}

bool CameraSample::isValid() const {
    return !_importance.isZero() && _pdfW > 0.0_r && std::isfinite(_pdfW);
}

} // namespace cadise