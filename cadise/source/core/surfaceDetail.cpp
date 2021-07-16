#include "core/surfaceDetail.h"

namespace cadise {

SurfaceDetail::SurfaceDetail() :
    _position(0.0_r),
    _uvw(0.0_r),
    _differentialGeometry(),
    _geometryLcs(),
    _shadingLcs() {
}

void SurfaceDetail::computeCoordinateSystem() {
    // compute geometry LCS
    {
        const Vector3R unitY = this->geometryNormal();
        const Vector3R xAxis = unitY.cross(_differentialGeometry.dPdU());
        if (!xAxis.isZero()) {
            const Vector3R unitX = xAxis.normalize();
            const Vector3R unitZ = unitX.cross(unitY);

            _geometryLcs.setAxes(
                unitX,
                unitY,
                unitZ);
        }
        else {
            _geometryLcs.initializeViaUnitY(unitY);
        }
    }

    // compute shading LCS
    {
        const Vector3R unitY = this->shadingNormal();
        const Vector3R xAxis = unitY.cross(_differentialGeometry.dNdU());
        if (!xAxis.isZero()) {
            const Vector3R unitX = xAxis.normalize();
            const Vector3R unitZ = unitX.cross(unitY);

            _shadingLcs.setAxes(
                unitX,
                unitY,
                unitZ);
        }
        else {
            _shadingLcs.initializeViaUnitY(unitY);
        }
    }
}

const Vector3R& SurfaceDetail::position() const {
    return _position;
}

const Vector3R& SurfaceDetail::geometryNormal() const {
    return _geometryLcs.yAxis();
}

const Vector3R& SurfaceDetail::shadingNormal() const {
    return _shadingLcs.yAxis();
}

const Vector3R& SurfaceDetail::uvw() const {
    return _uvw;
}

const LCS3R& SurfaceDetail::geometryLcs() const {
    return _geometryLcs;
}

const LCS3R& SurfaceDetail::shadingLcs() const {
    return _shadingLcs;
}

void SurfaceDetail::setPosition(const Vector3R& position) {
    _position = position;
}

void SurfaceDetail::setGeometryNormal(const Vector3R& geometryNormal) {
    _geometryLcs.setYAxis(geometryNormal);
}

void SurfaceDetail::setShadingNormal(const Vector3R& shadingNormal) {
    _shadingLcs.setYAxis(shadingNormal);
}

void SurfaceDetail::setUvw(const Vector3R& uvw) {
    _uvw = uvw;
}

void SurfaceDetail::setDifferentialGeometry(const DifferentialGeometry& differentialGeometry) {
    _differentialGeometry = differentialGeometry;
}

} // namespace cadise