#pragma once

#include "core/intersector/differentialGeometry.h"
#include "math/tLocalCoordinateSystem3.h"
#include "math/tVector.h"

namespace cadise {

class SurfaceDetail {
public:
    SurfaceDetail();

    void computeCoordinateSystem();

    const Vector3R& position() const;
    const Vector3R& geometryNormal() const;
    const Vector3R& shadingNormal() const;
    const Vector3R& uvw() const;
    const LCS3R& geometryLcs() const;
    const LCS3R& shadingLcs() const;

    void setGeometryNormal(const Vector3R& geometryNormal);
    void setShadingNormal(const Vector3R& shadingNormal);

    void setPosition(const Vector3R& point);
    void setUvw(const Vector3R& uvw);
    void setDifferentialGeometry(const DifferentialGeometry& differentialGeometry);

private:
    Vector3R _position;
    Vector3R _uvw;

    DifferentialGeometry _differentialGeometry;
    LCS3R                _geometryLcs;
    LCS3R                _shadingLcs;
};

} // namespace cadise