#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();

    const Vector3R& point() const;
    const Vector3R& geometryNormal() const;
    const Vector3R& shadingNormal() const;
    const Vector3R& frontNormal() const;
    const Vector3R& uvw() const;

    void setPoint(const Vector3R& point);
    void setGeometryNormal(const Vector3R& geometryNormal);
    void setShadingNormal(const Vector3R& shadingNormal);
    void setFrontNormal(const Vector3R& frontNormal);
    void setUvw(const Vector3R& uvw);

private:
    Vector3R _point;

    Vector3R _geometryNormal;
    Vector3R _shadingNormal;
    Vector3R _frontNormal;

    Vector3R _uvw;
};

} // namespace cadise