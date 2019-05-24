#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();
    SurfaceInfo(Vector3F point, Vector3F normal);

    Vector3F point();
    Vector3F normal();

    void setPoint(Vector3F point);
    void setNormal(Vector3F normal);

private:
    Vector3F _point;
    Vector3F _normal;
	
    // TODO
    // calculate differential geometry
    // such as dPdu, dPdv, dNdu, dNdv, etc
};

} // namespace cadise