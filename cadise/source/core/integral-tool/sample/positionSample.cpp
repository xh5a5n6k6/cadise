#include "core/integral-tool/sample/positionSample.h"

namespace cadise {

PositionSample::PositionSample() :
    _position(),
    _geometryNormal(),
    _shadingNormal(),
    _uvw(),
    _pdfA(0.0_r) {
}

const Vector3R& PositionSample::position() const {
    return _position;
}

const Vector3R& PositionSample::geometryNormal() const {
    return _geometryNormal;
}

const Vector3R& PositionSample::shadingNormal() const {
    return _shadingNormal;
}

const Vector3R& PositionSample::uvw() const {
    return _uvw;
}

real PositionSample::pdfA() const {
    return _pdfA;
}

void PositionSample::setPosition(const Vector3R& position) {
    _position = position;
}

void PositionSample::setGeometryNormal(const Vector3R& geometryNormal) {
    _geometryNormal = geometryNormal;
}

void PositionSample::setShadingNormal(const Vector3R& shadingNormal) {
    _shadingNormal = shadingNormal;
}

void PositionSample::setUvw(const Vector3R& uvw) {
    _uvw = uvw;
}

void PositionSample::setPdfA(const real pdfA) {
    _pdfA = pdfA;
}

bool PositionSample::isValid() const {
    return _pdfA > 0.0_r;
}

} // namespace cadise