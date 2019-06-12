#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();
    SurfaceInfo(const Vector3R point, const Vector3R normal);

    Vector3R point() const;
    Vector3R normal() const;

    void setPoint(const Vector3R point);
    void setNormal(const Vector3R normal);

private:
    Vector3R _point;
    Vector3R _normal;
	
    // TODO
    // calculate differential geometry
    // such as dPdu, dPdv, dNdu, dNdv, etc
};

} // namespace cadise