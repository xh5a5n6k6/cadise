#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();
    SurfaceInfo(Vector3R point, Vector3R normal);

    Vector3R point();
    Vector3R normal();

    void setPoint(Vector3R point);
    void setNormal(Vector3R normal);

private:
    Vector3R _point;
    Vector3R _normal;
	
    // TODO
    // calculate differential geometry
    // such as dPdu, dPdv, dNdu, dNdv, etc
};

} // namespace cadise