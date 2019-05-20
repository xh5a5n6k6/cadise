#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo {
public:
    SurfaceInfo();
    SurfaceInfo(Vector3 hitPoint, Vector3 hitNormal);

    Vector3 hitPoint();
    Vector3 hitNormal();

    void setHitPoint(Vector3 hitPoint);
    void setHitNormal(Vector3 hitNormal);

private:
    Vector3 _hitPoint;
    Vector3 _hitNormal;
	
    // TODO
    // calculate differential geometry
    // such as dPdu, dPdv, dNdu, dNdv, etc
};

} // namespace cadise