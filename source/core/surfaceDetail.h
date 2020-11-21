#pragma once

#include "math/tVector.h"

namespace cadise {

class SurfaceDetail {
public:
    SurfaceDetail();

    const Vector3R& position() const;
    const Vector3R& geometryNormal() const;
    const Vector3R& shadingNormal() const;
    const Vector3R& uvw() const;

    void setPosition(const Vector3R& point);
    void setGeometryNormal(const Vector3R& geometryNormal);
    void setShadingNormal(const Vector3R& shadingNormal);
    void setUvw(const Vector3R& uvw);

private:
    Vector3R _position;
    Vector3R _geometryNormal;
    Vector3R _shadingNormal;
    Vector3R _uvw;
};

} // namespace cadise