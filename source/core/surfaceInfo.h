#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();
    SurfaceInfo(Vector3 point, Vector3 normal);

    Vector3 point();
    Vector3 normal();

    void setPoint(Vector3 point);
    void setNormal(Vector3 normal);

private:
    Vector3 _point;
    Vector3 _normal;
	
    // TODO
    // calculate differential geometry
    // such as dPdu, dPdv, dNdu, dNdv, etc
};

} // namespace cadise