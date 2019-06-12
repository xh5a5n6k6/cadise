#pragma once

#include "core/surfaceInfo.h"

#include <memory>

namespace cadise {

class Intersector;

class Intersection{
public:
    Intersection();

    void setSurfaceInfo(const SurfaceInfo surfaceInfo);
    void setIntersector(const std::shared_ptr<Intersector> intersector);

    SurfaceInfo surfaceInfo() const;
    std::shared_ptr<Intersector> intersector() const;

private:
    SurfaceInfo _surfaceInfo;
    std::shared_ptr<Intersector> _intersector;
};

} // namespace cadise