#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class TextureMapper {
public:
    virtual Vector3R mappingToUvw(const SurfaceInfo& surfaceInfo) const = 0;
};

} // namespace cadise