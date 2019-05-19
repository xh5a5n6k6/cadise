#pragma once

#include "core/surfaceInfo.h"

#include <memory>

namespace cadise {

class Intersector;

class Intersection{
public:
    Intersection();

    void setSurfaceInfo(SurfaceInfo surfaceInfo);
    void setIntersector(std::shared_ptr<Intersector> intersector);

    SurfaceInfo& surfaceInfo();
    std::shared_ptr<Intersector> intersector();

private:
    SurfaceInfo _surfaceInfo;
    std::shared_ptr<Intersector> _intersector;
};

} // namespace cadise