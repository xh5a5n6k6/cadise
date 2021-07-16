#include "core/integral-tool/sample/cameraSample.h"

namespace cadise {

CameraSample::CameraSample() :
    _targetPosition(),
    _cameraPosition(),
    _cameraNormal(),
    _filmPosition(),
    _importance(0.0_r),
    _pdfW(0.0_r) {
}

const Vector3R& CameraSample::targetPosition() const {
    return _targetPosition;
}

const Vector3R& CameraSample::cameraPosition() const {
    return _cameraPosition;
}

const Vector3R& CameraSample::cameraNormal() const {
    return _cameraNormal;
}

const Vector2D& CameraSample::filmPosition() const {
    return _filmPosition;
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

void CameraSample::setCameraPosition(const Vector3R& cameraPosition) {
    _cameraPosition = cameraPosition;
}

void CameraSample::setCameraNormal(const Vector3R& cameraNormal) {
    _cameraNormal = cameraNormal;
}

void CameraSample::setFilmPosition(const Vector2D& filmPosition) {
    _filmPosition = filmPosition;
}

void CameraSample::setImportance(const Spectrum& importance) {
    _importance = importance;
}

void CameraSample::setPdfW(const real pdfW) {
    _pdfW = pdfW;
}

bool CameraSample::isValid() const {
    return !_importance.isZero() && _pdfW > 0.0_r && !_cameraNormal.isZero();
}

} // namespace cadise