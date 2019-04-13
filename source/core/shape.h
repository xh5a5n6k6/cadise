#pragma once

#include "core/ray.h"
#include "core/surfaceInfo.h"

namespace cadise {

class Shape {
public:
	virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) = 0;
	virtual bool isOccluded(Ray &ray) = 0;
};

} // namespace cadise