#pragma once

#include "core/primitive.h"
#include "core/surfaceInfo.h"

namespace cadise {

class Intersection{
public:
    Intersection();

    void setSurfaceInfo(SurfaceInfo surfaceInfo);
    void setPrimitive(Primitive primitive);

    SurfaceInfo& surfaceInfo();
    Primitive primitive();

private:
    SurfaceInfo _surfaceInfo;
    Primitive _primitive;
};

} // namespace cadise