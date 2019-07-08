#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceGeometryInfo {
public:
    SurfaceGeometryInfo();
    SurfaceGeometryInfo(const Vector3R& point, const Vector3R& normal);

    Vector3R point() const;
    Vector3R normal() const;

    void setPoint(const Vector3R& point);
    void setNormal(const Vector3R& normal);

private:
    Vector3R _point;
    Vector3R _normal;
};

} // namespace cadise