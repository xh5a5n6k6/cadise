#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();

    Vector3R point() const;
    Vector3R geometryNormal() const;
    Vector3R shadingNormal() const;
    Vector3R frontNormal() const;
    Vector3R uvw() const;

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